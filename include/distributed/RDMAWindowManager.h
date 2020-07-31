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

  static std::set<proc_id_t> GetSortedRanks() {
    std::set<proc_id_t> res;
    std::transform(
      std::begin(windows),
      std::end(windows),
      std::inserter(res, std::begin(res)),
      [](const auto & kv_pair){ return kv_pair.first; }
    );
    return res;
  }

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

  static void LockExclusive(const proc_id_t rank) {
    emp_assert(IsInitialized() || !IsInitializable());
    return windows.at(rank).LockExclusive();
  }

  static void LockShared(const proc_id_t rank) {
    emp_assert(IsInitialized() || !IsInitializable());
    return windows.at(rank).LockShared();
  }

  static void Unlock(const proc_id_t rank) {
    emp_assert(IsInitialized() || !IsInitializable());
    return windows.at(rank).Unlock();
  }

  template<typename T>
  static void Rput(
    const proc_id_t rank,
    const T *origin_addr,
    const MPI_Aint target_disp,
    MPI_Request *request
  ) {
    emp_assert(IsInitialized() || !IsInitializable());
    windows.at(rank).Rput<T>(origin_addr, target_disp, request);
  }

  static void Initialize(MPI_Comm comm=MPI_COMM_WORLD) {
    emp_assert(!IsInitialized());

    // sort ranks to prevent deadlock
    for (proc_id_t rank : GetSortedRanks()) {

      MPI_Comm dyad{
        group_to_comm(
          make_group({rank, get_rank(comm)}, comm_to_group(comm)),
          comm
        )
      };

      windows.at(rank).Initialize(
        translate_rank(rank, comm, dyad),
        dyad
      );

    }

    // ensure that RputDucts have received target offsets
    verify(MPI_Barrier(comm));

    emp_assert(IsInitialized() || !IsInitializable());
  }

  static void Cleanup() {
    // sort ranks to prevent deadlock
    for (proc_id_t rank : GetSortedRanks()) {
      windows.erase(rank);
    }
  }

};
