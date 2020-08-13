#pragma once

#include <stddef.h>

#include <mpi.h>

#include "mpi_utils.hpp"

namespace uit {

// TODO is it possible to have a seperate window/communicator
// between each pair of procs?
class RDMAWindow {

  char * buffer{nullptr};

  MPI_Win window;

  size_t size;

  // this is relative to the window communicator
  // rank where window is located
  proc_id_t local_rank;

public:

  ~RDMAWindow() {
    verify(MPI_Win_free(&window));
    verify(MPI_Free_mem(buffer));
  }

  bool IsInitialized() const { return buffer; }

  bool IsUninitialized() const { return !buffer; }

  bool IsInitializable() const { return size; }

  // TODO cache line alignment?
  size_t Acquire(const size_t num_bytes) {
    emp_assert(IsUninitialized());

    const size_t res = size;
    size += num_bytes;
    return res;

  }

  char * GetBytes(const size_t byte_offset) {
    emp_assert(IsInitialized());

    return std::next(
      reinterpret_cast<char *>(buffer),
      byte_offset
    );

  }

  const MPI_Win & GetWindow() {
    emp_assert(IsInitialized() || !IsInitializable());

    return window;
  }

  void LockExclusive() {

    emp_assert(IsInitialized() || !IsInitializable());

    verify(MPI_Win_lock(
      MPI_LOCK_EXCLUSIVE, // int lock_type
      // Indicates whether other processes may access the target window at the
      // same time (if MPI_LOCK_SHARED) or not (MPI_LOCK_EXCLUSIVE)
      local_rank, // int rank
      // rank of locked window (nonnegative integer)
      0, // int assert TODO optimize?
      // Used to optimize this call; zero may be used as a default.
      window // MPI_Win win
      // window object (handle)
    ));

  }

  void LockShared() {

    emp_assert(IsInitialized() || !IsInitializable());

    verify(MPI_Win_lock(
      MPI_LOCK_SHARED, // int lock_type
      // Indicates whether other processes may access the target window at the
      // same time (if MPI_LOCK_SHARED) or not (MPI_LOCK_EXCLUSIVE)
      local_rank, // int rank
      // rank of locked window (nonnegative integer)
      0, // int assert TODO optimize?
      // Used to optimize this call; zero may be used as a default.
      window // MPI_Win win
      // window object (handle)
    ));

  }

  void Unlock() {

    emp_assert(IsInitialized() || !IsInitializable());

    verify(MPI_Win_unlock(
      local_rank, // int rank
      // rank of window (nonnegative integer)
      window // MPI_Win win
      // window object (handle)
    ));

  }

  template<typename T>
  void Rput(
    const T *origin_addr,
    const MPI_Aint target_disp,
    MPI_Request *request
  ) {

    emp_assert(IsInitialized() || !IsInitializable());

    verify(MPI_Rput(
      origin_addr, // const void *origin_addr
      sizeof(T), // int origin_count
      MPI_BYTE, // MPI_Datatype origin_datatype
      local_rank, // int target_rank
      target_disp, // MPI_Aint target_disp
      // with MPI_Recv?, TODO factor in send_position offset?
      sizeof(T), // int target_count
      MPI_BYTE, // MPI_Datatype target_datatype
      window, // MPI_Win win
      request // MPI_Request* request (handle)
    ));

  }


  void Initialize(const proc_id_t target, MPI_Comm comm=MPI_COMM_WORLD) {
    emp_assert(IsUninitialized());

    local_rank = target;

    if (size) verify(MPI_Alloc_mem(
      size,
      MPI_INFO_NULL,
      &buffer
    ));

    // all procs must make this call
    verify(MPI_Win_create(
      buffer, // base: initial address of window (choice)
      size, // size: size of window in bytes (nonnegative integer)
      1, // disp_unit: local unit size for displacements, in bytes
         //   (positive integer)
      MPI_INFO_NULL, // info: info argument (handle)
      comm, // comm: communicator (handle)
      &window // win: window object returned by the call (handle)
    ));

    // ensure that RputDucts have received target offsets
    verify(MPI_Barrier(comm));

    emp_assert(IsInitialized() || !IsInitializable());

  }

  size_t GetSize() const { return size; }

  proc_id_t GetLocalRank() const { return local_rank; }

  std::string ToString() const {

    std::stringstream ss;
    ss << format_member("IsInitialized()", emp::to_string(IsInitialized()))
      << std::endl;
    ss << format_member("IsUninitialized()", emp::to_string(IsUninitialized()))
      << std::endl;
    ss << format_member("IsInitializable()", emp::to_string(IsInitializable()))
      << std::endl;
    // TODO add print function for MPI_Win
    ss << format_member("char * buffer", static_cast<const void *>(buffer))
      << std::endl;
    ss << format_member("size_t size", size) << std::endl;
    ss << format_member("proc_id_t local_rank", local_rank);

    return ss.str();

  }


};

}
