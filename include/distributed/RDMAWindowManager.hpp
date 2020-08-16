#pragma once

#include <unordered_map>
#include <thread>
#include <set>
#include <stddef.h>
#include <unordered_map>

#include <mpi.h>

#include "group_utils.hpp"
#include "mpi_utils.hpp"
#include "RDMAWindow.hpp"

namespace uit {

// TODO is it possible to have a seperate window/communicator
// between each pair of procs?
class RDMAWindowManager {

  std::unordered_map<proc_id_t, RDMAWindow> windows{};
  mutable std::mutex mutex;

  std::set<proc_id_t> GetSortedRanks() {
    std::set<proc_id_t> res;
    std::transform(
      std::begin(windows),
      std::end(windows),
      std::inserter(res, std::begin(res)),
      [](const auto & kv_pair){ return kv_pair.first; }
    );
    return res;
  }

  bool IsInitialized() {
    return std::any_of(
      std::begin(windows),
      std::end(windows),
      [](const auto & key_value){
        const auto & [rank, window] = key_value;
        return window.IsInitialized();
      }
    );
  }

  bool IsInitializable() {
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

  ~RDMAWindowManager() {
    // sort ranks to prevent deadlock
    for (proc_id_t rank : GetSortedRanks()) {
      windows.erase(rank);
    }
  }

  // TODO cache line alignment?
  size_t Acquire(const proc_id_t rank, const size_t num_bytes) {

    // make this call thread safe
    const std::lock_guard guard{mutex};

    emp_assert(!IsInitialized());

    return windows[rank].Acquire(num_bytes);

  }

  char * GetBytes(const proc_id_t rank, const size_t byte_offset) {
    emp_assert(IsInitialized());
    emp_assert(
      windows.count(rank),
      rank,
      ToString()
    );

    return windows.at(rank).GetBytes(byte_offset);

  }

  const MPI_Win & GetWindow(const proc_id_t rank) {
    emp_assert(IsInitialized());
    emp_assert(
      windows.count(rank),
      rank,
      ToString()
    );
    return windows.at(rank).GetWindow();
  }

  void LockExclusive(const proc_id_t rank) {
    emp_assert(IsInitialized() || !IsInitializable());
    emp_assert(
      windows.count(rank),
      rank,
      ToString()
    );
    return windows.at(rank).LockExclusive();
  }

  void LockShared(const proc_id_t rank) {
    emp_assert(IsInitialized() || !IsInitializable());
    emp_assert(
      windows.count(rank),
      rank,
      ToString()
    );
    return windows.at(rank).LockShared();
  }

  void Unlock(const proc_id_t rank) {
    emp_assert(IsInitialized() || !IsInitializable());
    emp_assert(
      windows.count(rank),
      rank,
      ToString()
    );
    return windows.at(rank).Unlock();
  }

  template<typename T>
  void Rput(
    const proc_id_t rank,
    const T *origin_addr,
    const MPI_Aint target_disp,
    MPI_Request *request
  ) {
    emp_assert(IsInitialized() || !IsInitializable());
    emp_assert(
      windows.count(rank),
      rank,
      ToString()
    );
    windows.at(rank).Rput<T>(origin_addr, target_disp, request);
  }

  void Initialize(MPI_Comm comm=MPI_COMM_WORLD) {
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

  std::string ToString() {

    std::stringstream ss;
    ss << format_member("windows.size()", windows.size()) << std::endl;

    for (proc_id_t rank : GetSortedRanks()) {
      ss << format_member("rank", rank) << std::endl;
      ss << format_member("window", windows.at(rank).ToString()) << std::endl;
    }

    return ss.str();

  }

};

} // namespace uit
