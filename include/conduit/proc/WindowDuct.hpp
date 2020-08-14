#pragma once

#include <algorithm>
#include <array>
#include <stddef.h>

#include <mpi.h>

#include "../../../third-party/Empirical/source/base/assert.h"
#include "../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../distributed/mpi_utils.hpp"
#include "../../distributed/RDMAWindowManager.hpp"
#include "../../utility/CircularIndex.hpp"
#include "../../utility/identity.hpp"
#include "../../utility/print_utils.hpp"

#include "../config.hpp"

#include "InterProcAddress.hpp"
#include "SharedBackEnd.hpp"

namespace uit {

template<typename ImplSpec>
class Duct;

template<typename ImplSpec>
class WindowDuct {

  friend Duct<ImplSpec>;

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  const uit::InterProcAddress address;
  std::shared_ptr<uit::SharedBackEnd<ImplSpec>> back_end;

  const int byte_offset;

public:

  WindowDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<uit::SharedBackEnd<ImplSpec>> back_end_
  ) : address(address_)
  , back_end(back_end_)
  , byte_offset(
    address.GetOutletProc() == uit::get_rank(address.GetComm())
      ? back_end->GetWindowManager().Acquire(
        address.GetInletProc(), sizeof(T) /** N*/
      ) : -1
  ) {
    if (address.GetOutletProc() == uit::get_rank(address.GetComm())) {
      MPI_Request req;
      verify(MPI_Isend(
        &byte_offset, // const void *buf
        1, // int count
        MPI_INT, // MPI_Datatype datatype
        address.GetInletProc(), // int dest
        address.GetTag(), // int tag
        address.GetComm(), // MPI_Comm comm
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
    back_end->GetWindowManager().LockShared(address.GetInletProc());
    std::memcpy(
      &res,
      back_end->GetWindowManager().GetBytes(
        address.GetInletProc(), byte_offset /*+ sizeof(T) * n*/
      ),
      sizeof(T)
    );
    back_end->GetWindowManager().Unlock(address.GetInletProc());

    return res;
  }

  const void * GetPosition(const size_t n) const {
    return back_end->GetWindowManager().GetBytes(
      address.GetInletProc(),
      byte_offset /*+ sizeof(T) * n*/
    );
  }

  [[noreturn]] void SetElement(const size_t n, const T & val) {
    throw "bad SetElement on WindowDuct";
  }

  static std::string GetType() { return "WindowDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("int get_rank()", get_rank()) << std::endl;
    // ss << format_member("size_t receive_position", receive_position);
    return ss.str();
  }

};

}
