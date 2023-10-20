#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_OUTLET_ACCUMULATING_TYPE_TRIVIAL_T__IRECVDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_OUTLET_ACCUMULATING_TYPE_TRIVIAL_T__IRECVDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <cassert>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../uitsl/debug/uitsl_always_assert.hpp"
#include "../../../../../../uitsl/distributed/MsgAccumulatorPacket.hpp"
#include "../../../../../../uitsl/meta/t::static_test.hpp"
#include "../../../../../../uitsl/mpi/mpi_init_utils.hpp"
#include "../../../../../../uitsl/mpi/Request.hpp"
#include "../../../../../../uitsl/utility/print_utils.hpp"

#include "../../../../../setup/InterProcAddress.hpp"

#include "../../backend/MockBackEnd.hpp"

namespace uit {
namespace t {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class IrecvDuct {

public:

  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  static_assert( uitsl::t::static_test<T>(), uitsl_t_message );

  using packet_t = uitsl::MsgAccumulatorPacket<T>;
  packet_t buffer{};
  packet_t cache{};

  uitsl::Request receive_request;

  const uit::InterProcAddress address;

  void PostReceiveRequest() {
    assert( uitsl::test_null( receive_request ) );
    UITSL_Irecv(
      &buffer,
      sizeof(packet_t),
      MPI_BYTE,
      address.GetInletProc(),
      address.GetTag(),
      address.GetComm(),
      &receive_request
    );
    assert( !uitsl::test_null( receive_request ) );
  }

  void CancelReceiveRequest() {
    assert( !uitsl::test_null( receive_request ) );

    UITSL_Cancel( &receive_request );
    UITSL_Request_free( &receive_request );

    assert( uitsl::test_null( receive_request ) );
  }

  // returns true if receive was full
  bool TryReceive() {

    assert( !uitsl::test_null( receive_request ) );

    const bool res = uitsl::test_completion( receive_request );

    if (res) {
      cache += buffer;
      PostReceiveRequest();
    }

    return res;

  }

  void FlushReceives() { while ( TryReceive() ); }

public:

  IrecvDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end
  ) : address(address_) { PostReceiveRequest(); }

  ~IrecvDuct() {
    FlushReceives();
    CancelReceiveRequest();
    assert( uitsl::test_null( receive_request ) );
  }

  [[noreturn]] bool TryPut(const T&) const {
    uitsl_always_assert(false, "TryPut called on IrecvDuct");
    __builtin_unreachable();
  }

  [[noreturn]] bool TryFlush() const {
    uitsl_always_assert(false, "Flush called on IrecvDuct");
    __builtin_unreachable();
  }

  /**
   * TODO.
   *
   * @param num_requested TODO.
   * @return number items consumed.
   */
  size_t TryConsumeGets(const size_t num_requested) {

    assert( num_requested == std::numeric_limits<size_t>::max() );

    cache = packet_t{};

    FlushReceives();

    return cache.GetEpoch();

  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return cache.GetData(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { return cache.GetData(); }

  static std::string GetName() { return "IrecvDuct"; }

  static constexpr bool CanStep() { return false; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    ss << uitsl::format_member("buffer_t buffer", buffer[0]) << '\n';
    ss << uitsl::format_member("InterProcAddress address", address) << '\n';
    return ss.str();
  }

};

} // namespace t
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_OUTLET_ACCUMULATING_TYPE_TRIVIAL_T__IRECVDUCT_HPP_INCLUDE
