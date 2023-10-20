#pragma once
#ifndef UITSL_DISTRIBUTED_RDMAWINDOWMANAGER_HPP_INCLUDE
#define UITSL_DISTRIBUTED_RDMAWINDOWMANAGER_HPP_INCLUDE

#include <cassert>
#include <cstddef>
#include <set>
#include <stddef.h>
#include <thread>
#include <unordered_map>
#include <vector>

#include <mpi.h>

#include "../mpi/group_utils.hpp"
#include "../mpi/mpi_init_utils.hpp"

#include "RdmaWindow.hpp"

namespace uitsl {

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
    const std::vector<std::byte>& initial_bytes
  ) {

    // make this call thread safe
    const std::lock_guard guard{mutex};

    assert( !IsInitialized() );

    return windows[rank].Acquire(initial_bytes);

  }

  std::byte *GetBytes(const proc_id_t rank, const size_t byte_offset) {
    assert( IsInitialized() );
    assert( windows.count(rank) );

    return windows.at(rank).GetBytes(byte_offset);

  }

  const MPI_Win& GetWindow(const proc_id_t rank) {
    assert( IsInitialized() );
    assert( windows.count(rank) );
    return windows.at(rank).GetWindow();
  }

  void LockExclusive(const proc_id_t rank) {
    assert( IsInitialized() );
    assert( windows.count(rank) );
    return windows.at(rank).LockExclusive();
  }

  void LockShared(const proc_id_t rank) {
    assert( IsInitialized() );
    assert( windows.count(rank) );
    return windows.at(rank).LockShared();
  }

  void Unlock(const proc_id_t rank) {
    assert( IsInitialized() );
    assert( windows.count(rank) );
    return windows.at(rank).Unlock();
  }

  void Put(
    const proc_id_t rank,
    const std::byte *origin_addr,
    const size_t num_bytes,
    const MPI_Aint target_disp
  ) {
    assert( IsInitialized() );
    assert( windows.count(rank) );
    windows.at(rank).Put(origin_addr, num_bytes, target_disp);
  }

  void Rput(
    const proc_id_t rank,
    const std::byte *origin_addr,
    const size_t num_bytes,
    const MPI_Aint target_disp,
    MPI_Request *request
  ) {
    assert( IsInitialized() );
    assert( windows.count(rank) );
    assert( uitsl::test_null(*request) );
    windows.at(rank).Rput(origin_addr, num_bytes, target_disp, request);
  }

  template<typename T>
  void Accumulate(
    const proc_id_t rank,
    const std::byte *origin_addr,
    const size_t num_bytes,
    const MPI_Aint target_disp
  ) {
    assert( IsInitialized() );
    assert( windows.count(rank) );
    windows.at(rank).Accumulate<T>(origin_addr, num_bytes, target_disp);
  }

  template<typename T>
  void Raccumulate(
    const proc_id_t rank,
    const std::byte *origin_addr,
    const size_t num_bytes,
    const MPI_Aint target_disp,
    MPI_Request *request
  ) {
    assert( IsInitialized() );
    assert( windows.count(rank) );
    assert( uitsl::test_null(*request) );
  windows.at(
      rank
    ).Raccumulate<T>(origin_addr, num_bytes, target_disp, request);
  }

  void Initialize(MPI_Comm comm=MPI_COMM_WORLD) {
    assert(!IsInitialized());

    // sort ranks to prevent deadlock
    for (proc_id_t rank : GetSortedRanks()) {

      MPI_Comm dyad{
        uitsl::group_to_comm(
          uitsl::make_group(
            {rank, uitsl::get_rank(comm)},
            uitsl::comm_to_group(comm)
          ),
          comm
        )
      };

      windows.at(rank).Initialize(
        uitsl::translate_comm_rank(rank, comm, dyad),
        dyad
      );

    }

    // ensure that RputDucts have received target offsets
    UITSL_Barrier(comm);

    assert(windows.empty() || IsInitialized());
  }

  std::string ToString() {

    std::stringstream ss;
    ss << uitsl::format_member("windows.size()", windows.size()) << '\n';

    for (proc_id_t rank : GetSortedRanks()) {
      ss << uitsl::format_member("rank", rank) << '\n';
      ss << uitsl::format_member("window", windows.at(rank).ToString()) << '\n';
    }

    return ss.str();

  }

};

} // namespace uitsl

#endif // #ifndef UITSL_DISTRIBUTED_RDMAWINDOWMANAGER_HPP_INCLUDE
