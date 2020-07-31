#pragma once

#include "mpi.h"

#include <algorithm>
#include <array>

#include "base/assert.h"
#include "tools/string_utils.h"

#include "../../distributed/RDMAWindowManager.h"
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

  const int outlet_proc;

  const int inlet_proc;

  const int byte_offset;

public:

  WindowDuct(
    const int outlet_proc_,
    const int inlet_proc_,
    const int tag_=0,
    MPI_Comm comm_=MPI_COMM_WORLD
  ) : outlet_proc(outlet_proc_)
  , inlet_proc(inlet_proc_)
  , byte_offset(
    outlet_proc == get_rank(comm_)
      ? RDMAWindowManager::Acquire(inlet_proc, sizeof(T) /** N*/)
      : -1
  ) {
    if (outlet_proc == get_rank(comm_)) {
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
    RDMAWindowManager::LockShared(inlet_proc);
    std::memcpy(
      &res,
      RDMAWindowManager::GetBytes(inlet_proc, byte_offset /*+ sizeof(T) * n*/),
      sizeof(T)
    );
    RDMAWindowManager::Unlock(inlet_proc);

    return res;
  }

  const void * GetPosition(const size_t n) const {
    return RDMAWindowManager::GetBytes(
      inlet_proc,
      byte_offset /*+ sizeof(T) * n*/
    );
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
