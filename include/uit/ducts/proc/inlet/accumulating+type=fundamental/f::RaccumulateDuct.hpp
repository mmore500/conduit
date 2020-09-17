#pragma once

#include <algorithm>
#include <array>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../third-party/Empirical/source/base/vector.h"
#include "../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../uitsl/distributed/RdmaWindowManager.hpp"
#include "../../../../../uitsl/distributed/RdmaAccumulatorPacket.hpp"
#include "../../../../../uitsl/mpi/Request.hpp"
#include "../../../../../uitsl/nonce/CircularIndex.hpp"
#include "../../../../../uitsl/utility/print_utils.hpp"
#include "../../../../../uitsl/debug/WarnOnce.hpp"

#include "../../../../setup/InterProcAddress.hpp"

#include "../../backend/RdmaBackEnd.hpp"

namespace uit {
namespace f {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class RaccumulateDuct {

public:

  using BackEndImpl = uit::RdmaBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};
  using packet_t = uitsl::RdmaAccumulatorPacket<T>;

  uitsl::Request request{};
  packet_t buffer{};

  const uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  uitsl::Request target_offset_request;
  int target_offset;

  void DoPostAccumulate() {

    emp_assert( uitsl::test_null( request ) );

    // TODO FIXME what kind of lock is needed here?
    back_end->GetWindowManager().LockShared( address.GetOutletProc() );

    back_end->GetWindowManager().template Raccumulate<T>(
      address.GetOutletProc(),
      reinterpret_cast<const std::byte*>( &buffer ),
      sizeof(packet_t),
      target_offset,
      &request
    );

    back_end->GetWindowManager().Unlock( address.GetOutletProc() );

    emp_assert( !uitsl::test_null( request ) );

    buffer = packet_t{};

  }

  void TryPostAccumulate() {
    if (uitsl::test_completion( request )) DoPostAccumulate();
  }

public:

  RaccumulateDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_
  ) : address(address_)
  , back_end(back_end_)
  {

    if (uitsl::get_rank(address.GetComm()) == address.GetInletProc()) {
      // make spoof call to ensure reciporical activation
      back_end->GetWindowManager().Acquire(
        address.GetOutletProc(),
        emp::vector<std::byte>{}
      );

      // we'll emp_assert later to make sure it actually completed
      UITSL_Irecv(
        &target_offset, // void *buf
        1, // int count
        MPI_INT, // MPI_Datatype datatype
        address.GetOutletProc(), // int source
        address.GetTag(), // int tag
        address.GetComm(), // MPI_Comm comm
        &target_offset_request // MPI_Request *request
      );
    }

  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    buffer.data += val;
    ++buffer.epoch;
    TryPostAccumulate();
    return true;
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() const { return true; }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    throw "ConsumeGets called on RaccumulateDuct";
  }

  [[noreturn]] const T& Get() const { throw "Get called on RaccumulateDuct"; }

  [[noreturn]] T& Get() { throw "Get called on RaccumulateDuct"; }

  static std::string GetType() { return "RaccumulateDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << uitsl::format_member("InterProcAddress address", address) << std::endl;
    return ss.str();
  }

};

} // namespace f
} // namespace uit
