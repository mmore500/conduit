#pragma once

#include "mpi.h"

#include <algorithm>
#include <array>

#include "base/assert.h"
#include "tools/string_utils.h"

#include "../../distributed/RDMAWindow.h"
#include "../../utility/CircularIndex.h"
#include "../../utility/identity.h"

#include "../config.h"
#include "../../distributed/mpi_utils.h"
#include "../../utility/print_utils.h"

template<typename T, size_t N>
class Duct;

// TODO rename WindowDuct
template<typename T, size_t N=DEFAULT_BUFFER>
class WindowDuct {

  friend Duct<T, N>;

  const int byte_offset;

public:

  WindowDuct(
    const int outlet_proc,
    const int inlet_proc_,
    const int tag_=0,
    MPI_Comm comm_=MPI_COMM_WORLD
  ) : byte_offset(
    outlet_proc == get_rank()
      ? RDMAWindow::Acquire(sizeof(T) /** N*/)
      : -1
  ) {
    if (outlet_proc == get_rank()) {
      MPI_Request req;
      verify(MPI_Isend(
        &byte_offset, // const void *buf
        1, // int count
        MPI_INT, // MPI_Datatype datatype
        inlet_proc_, // int dest
        tag_, // int tag
        comm_, // MPI_Comm comm
        &req // MPI_Request * request
      ));
      MPI_Request_free(&req); //TODO test for completion in destructor?
    }
  }

  void Initialize(const size_t write_position) { ; }

  //todo rename
  [[noreturn]] void Push() { throw "bad Push on WindowDuct"; }

  //todo rename
  void Pop(const size_t count) { ; }

  size_t GetPending() { return 1; }

  [[noreturn]] size_t GetAvailableCapacity() {
    throw "bad GetAvailableCapacity on WindowDuct";
  }

  T GetElement(const size_t n) const {
    T res;
    // TODO use atomics as counter?
    // TODO just do a RDMA Get or something on own window?
    // TODO move this all into RDMAWindow and rely on intermittent calls there
    // to get latest?
    // lock own window
    verify(MPI_Win_lock(
      MPI_LOCK_SHARED, // int lock_type TODO shared?
      // Indicates whether other processes may access the target window at the /
      // same time (if MPI_LOCK_SHARED) or not (MPI_LOCK_EXCLUSIVE)
      get_rank(), // int rank
      // rank of locked window (nonnegative integer)
      0, // int assert TODO optimize?
      // Used to optimize this call; zero may be used as a default.
      RDMAWindow::GetWindow()// MPI_Win win
      // window object (handle)
    ));
    // get latest value
    // TODO somehow use Rget?
    std::memcpy(
      &res,
      RDMAWindow::GetBytes(byte_offset /*+ sizeof(T) * n*/),
      sizeof(T)
    );
    // unlock own window
    verify(MPI_Win_unlock(
      get_rank(), // int rank
      // rank of window (nonnegative integer)
      RDMAWindow::GetWindow() // MPI_Win win
      // window object (handle)
    ));

    return res;
  }

  const void * GetPosition(const size_t n) const {
    return RDMAWindow::GetBytes(byte_offset /*+ sizeof(T) * n*/);
  }

  [[noreturn]] void SetElement(const size_t n, const T & val) {
    throw "bad SetElement on WindowDuct";
  }

  std::string GetType() const { return "WindowDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("int get_rank()", get_rank()) << std::endl;
    // ss << format_member("size_t receive_position", receive_position);
    return ss.str();
  }

};
