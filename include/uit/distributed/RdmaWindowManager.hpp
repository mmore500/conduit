#pragma once

#include <cstddef>
#include <unordered_map>
#include <thread>
#include <set>
#include <stddef.h>
#include <unordered_map>

#include <mpi.h>

#include "group_utils.hpp"
#include "mpi_utils.hpp"
#include "RdmaWindow.hpp"

namespace uit {

// TODO is it possible to have a seperate window/communicator
// between each pair of procs?
class RdmaWindowManager {

  std::unordered_map<proc_id_t, RdmaWindow> windows{};
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

public:

  ~RdmaWindowManager() {
    // sort ranks to prevent deadlock
    for (proc_id_t rank : GetSortedRanks()) {
      windows.erase(rank);
    }
  }

  // TODO cache line alignment?
  size_t Acquire(
    const proc_id_t rank,
    const emp::vector<std::byte>& initial_bytes
  ) {

    // make this call thread safe
    const std::lock_guard guard{mutex};

    emp_assert( !IsInitialized() );

    return windows[rank].Acquire(initial_bytes);

  }

  std::byte *GetBytes(const proc_id_t rank, const size_t byte_offset) {
    emp_assert( IsInitialized() );
    emp_assert( windows.count(rank) );

    return windows.at(rank).GetBytes(byte_offset);

  }

  const MPI_Win& GetWindow(const proc_id_t rank) {
    emp_assert( IsInitialized() );
    emp_assert( windows.count(rank) );
    return windows.at(rank).GetWindow();
  }

  void LockExclusive(const proc_id_t rank) {
    emp_assert( IsInitialized() );
    emp_assert( windows.count(rank) );
    return windows.at(rank).LockExclusive();
  }

  void LockShared(const proc_id_t rank) {
    emp_assert( IsInitialized() );
    emp_assert( windows.count(rank) );
    return windows.at(rank).LockShared();
  }

  void Unlock(const proc_id_t rank) {
    emp_assert( IsInitialized() );
    emp_assert( windows.count(rank) );
    return windows.at(rank).Unlock();
  }

  void Rput(
    const proc_id_t rank,
    const std::byte *origin_addr,
    const size_t num_bytes,
    const MPI_Aint target_disp,
    MPI_Request *request
  ) {
    emp_assert( IsInitialized() );
    emp_assert( windows.count(rank) );
    windows.at(rank).Rput(origin_addr, num_bytes, target_disp, request);
  }

  template<typename T>
  void Accumulate(
    const proc_id_t rank,
    const std::byte *origin_addr,
    const size_t num_bytes,
    const MPI_Aint target_disp
  ) {
    emp_assert( IsInitialized() );
    emp_assert( windows.count(rank) );
    windows.at(rank).Accumulate<T>(origin_addr, num_bytes, target_disp);
  }

  void Initialize(MPI_Comm comm=MPI_COMM_WORLD) {
    emp_assert(!IsInitialized());

    // sort ranks to prevent deadlock
    for (proc_id_t rank : GetSortedRanks()) {

      MPI_Comm dyad{
        uit::group_to_comm(
          uit::make_group(
            {rank, uit::get_rank(comm)},
            uit::comm_to_group(comm)
          ),
          comm
        )
      };

      windows.at(rank).Initialize(
        uit::translate_comm_rank(rank, comm, dyad),
        dyad
      );

    }

    // ensure that RputDucts have received target offsets
    UIT_Barrier(comm);

    emp_assert(windows.empty() || IsInitialized());
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
