#pragma once

#include <algorithm>
#include <memory>
#include <tuple>
#include <utility>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../../distributed/mpi_utils.hpp"
#include "../../../../../distributed/Request.hpp"
#include "../../../../../utility/CircularIndex.hpp"
#include "../../../../../utility/identity.hpp"
#include "../../../../../utility/print_utils.hpp"

#include "../../../../InterProcAddress.hpp"

#include "../../../backend/MockBackEnd.hpp"

namespace uit {
namespace internal {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImmediateSendFunctor, typename ImplSpec>
class DequeImmediateSendDuct {

public:

  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;

  // newest requests are pushed back, oldest requests are at front
  std::deque<std::tuple<T, uit::Request>> buffer;

  const uit::InterProcAddress address;

  void PostSend() {
    emp_assert( uit::test_null(
      std::get<uit::Request>(buffer.back())
    ) );

    ImmediateSendFunctor{}(
      &std::get<T>(buffer.back()),
      sizeof(T),
      MPI_BYTE,
      address.GetOutletProc(),
      address.GetTag(),
      address.GetComm(),
      &std::get<uit::Request>(buffer.back())
    );

  }

  bool TryFinalizeSend() {
    emp_assert( !uit::test_null( std::get<uit::Request>(buffer.front()) ) );

    if (uit::test_completion( std::get<uit::Request>(buffer.front()) )) {
      emp_assert( uit::test_null( std::get<uit::Request>(buffer.front()) ) );
      buffer.pop_front();
      return true;
    } else return false;

  }

  void CancelPendingSend() {
    emp_assert( !uit::test_null( std::get<uit::Request>(buffer.front()) ) );

    UIT_Cancel( &std::get<uit::Request>(buffer.front()) );
    UIT_Request_free( &std::get<uit::Request>(buffer.front()) );

    emp_assert( uit::test_null( std::get<uit::Request>(buffer.front()) ) );

    buffer.pop_front();
  }

  void FlushFinalizedSends() { while (buffer.size() && TryFinalizeSend()); }

public:

  DequeImmediateSendDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end
  ) : address(address_)
  { }

  ~DequeImmediateSendDuct() {
    FlushFinalizedSends();
    while (buffer.size()) CancelPendingSend();
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    buffer.emplace_back(
      val,
      uit::Request{}
    );
    emp_assert( uit::test_null( std::get<uit::Request>(buffer.back()) ) );
    PostSend();
    return true;
  }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    throw "ConsumeGets called on DequeImmediateSendDuct";
  }

  [[noreturn]] const T& Get() const {
    throw "Get called on DequeImmediateSendDuct";
  }

  [[noreturn]] T& Get() {
    throw "Get called on DequeImmediateSendDuct";
  }

  static std::string GetType() { return "DequeImmediateSendDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("InterProcAddress address", address) << std::endl;
    return ss.str();
  }

};

} // namespace internal
} // namespace uit
