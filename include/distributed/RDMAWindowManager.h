#pragma once

#include <set>
#include <unordered_map>

#include "mpi.h"

#include "RDMAWindow.h"
#include "mpi_utils.h"
#include "group_utils.h"

#include <unordered_map>

// TODO is it possible to have a seperate window/communicator
// between each pair of procs?
class RDMAWindowManager {

  inline static std::unordered_map<proc_id_t, RDMAWindow> windows{};

  static bool IsInitialized() {
    return std::any_of(
      std::begin(windows),
      std::end(windows),
      [](const auto & key_value){
        const auto & [rank, window] = key_value;
        return window.IsInitialized();
      }
    );
  }

  static bool IsInitializable() {
    return std::any_of(
      std::begin(windows),
      std::end(windows),
      [](const auto & key_value){
        const auto & [rank, window] = key_value;
        return window.IsInitializable();
      }
    );
  }

public:

  // TODO cache line alignment?
  static size_t Acquire(const proc_id_t rank, const size_t num_bytes) {
    emp_assert(!IsInitialized());

    return windows[rank].Acquire(num_bytes);

  }

  static char * GetBytes(const proc_id_t rank, const size_t byte_offset) {
    emp_assert(IsInitialized());

    return windows.at(rank).GetBytes(byte_offset);

  }

  static const MPI_Win & GetWindow(const proc_id_t rank) {
    emp_assert(IsInitialized());
    return windows.at(rank).GetWindow();
  }

  static void Initialize(MPI_Comm comm=MPI_COMM_WORLD) {
    emp_assert(!IsInitialized());

    // sort ranks to prevent deadlock
    std::set<int> sorted_ranks;
    std::transform(
      std::begin(windows),
      std::end(windows),
      std::inserter(sorted_ranks, std::begin(sorted_ranks)),
      [](const auto & kv_pair){ return kv_pair.first; }
    );

    for (int rank : sorted_ranks) {

      MPI_Comm dyad{
        group_to_comm(
          make_group({rank, get_rank(comm)}, comm_to_group(comm)),
          comm
        )
      };

      windows.at(rank).Initialize(dyad);

    }

    // ensure that RputDucts have received target offsets
    verify(MPI_Barrier(comm));

    emp_assert(IsInitialized() || !IsInitializable());
  }

  static void Cleanup() {
    windows.clear();
  }

};
