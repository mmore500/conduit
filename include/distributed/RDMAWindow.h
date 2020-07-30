#pragma once

#include "mpi.h"

// TODO is it possible to have a seperate window/communicator
// between each pair of procs?
class RDMAWindow {

  char * buffer{nullptr};

  MPI_Win window;

  size_t size;

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

  void Initialize(MPI_Comm comm_=MPI_COMM_WORLD) {
    emp_assert(IsUninitialized());

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

};
