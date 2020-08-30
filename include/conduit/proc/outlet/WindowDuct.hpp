#pragma once

#include <algorithm>
#include <array>
#include <stddef.h>

#include <mpi.h>

#include "../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../distributed/mpi_utils.hpp"
#include "../../../distributed/RDMAWindowManager.hpp"
#include "../../../utility/CircularIndex.hpp"
#include "../../../utility/identity.hpp"
#include "../../../utility/print_utils.hpp"
#include "../../../utility/WarnOnce.hpp"

#include "../../config.hpp"
#include "../../InterProcAddress.hpp"

#include "../backend/RdmaBackEnd.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class WindowDuct {

public:

  using BackEndImpl = uit::internal::RdmaBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  const uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;
  const int byte_offset;

  T res{};

  size_t CountUnconsumedGets() const { return 1; }

public:

  WindowDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_
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
      uit::verify(MPI_Isend(
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

    static const uit::WarnOnce warning{
      "WindowDuct is experimental and may be unreliable"
    };

  }

  void Put(const T& val) { throw "Put called on WindowDuct"; }

  [[noreturn]] bool IsReadyForPut() {
    throw "IsReadyForPut called on WindowDuct";
  }

  size_t TryConsumeGets(const size_t requested) const { return 1; }

  const T& Get() {
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

  static std::string GetName() { return "WindowDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("int get_rank()", get_rank()) << std::endl;
    return ss.str();
  }

};

} // namespace uit
