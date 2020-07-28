#pragma once

#include "mpi.h"

// TODO is it possible to have a seperate window/communicator
// between each pair of procs?
class RDMAWindow {

  static inline char * buffer{nullptr};

  static inline MPI_Win window{};

  static inline size_t size;

public:

  ~RDMAWindow() {
    verify(MPI_Win_free(&window));
    verify(MPI_Free_mem(buffer));
  }

  static bool IsInitialized() { return buffer; }

  // TODO cache line alignment?
  static size_t Acquire(const size_t num_bytes) {

    emp_assert(!IsInitialized(), buffer);
    const size_t res = size;
    size += num_bytes;
    return res;

  }

  static char * GetBytes(const size_t byte_offset) {

    emp_assert(IsInitialized(), buffer);
    return std::next(
      reinterpret_cast<char *>(buffer),
      byte_offset
    );

  }

  static const MPI_Win & GetWindow() {
    emp_assert(IsInitialized(), buffer);
    return window;
  }

  static void Initialize(MPI_Comm comm=MPI_COMM_WORLD) {
    emp_assert(
      !IsInitialized(),
      emp::to_string(reinterpret_cast<void *>(buffer))
    );

    verify(MPI_Alloc_mem(
      size,
      MPI_INFO_NULL,
      &buffer
    ));

    verify(MPI_Win_create(
      buffer, // base: initial address of window (choice)
      size, // size: size of window in bytes (nonnegative integer)
      1, // disp_unit: local unit size for displacements, in bytes
         //   (positive integer)
      MPI_INFO_NULL, // info: info argument (handle)
      comm, // comm: communicator (handle)
      &window // win: window object returned by the call (handle)
    ));
  }

};
