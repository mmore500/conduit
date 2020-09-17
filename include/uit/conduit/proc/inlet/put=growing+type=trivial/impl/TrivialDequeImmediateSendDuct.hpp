#pragma once

#include <algorithm>
#include <memory>
#include <tuple>
#include <utility>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../../uitsl/mpi/Request.hpp"
#include "../../../../../../uitsl/nonce/CircularIndex.hpp"
#include "../../../../../../uitsl/utility/print_utils.hpp"

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
class TrivialDequeImmediateSendDuct {

public:

  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;

  // newest requests are pushed back, oldest requests are at front
  std::deque<std::tuple<T, uitsl::Request>> buffer;

  const uit::InterProcAddress address;

  void PostSend() {
    emp_assert( uitsl::test_null(
      std::get<uitsl::Request>(buffer.back())
    ) );

    ImmediateSendFunctor{}(
      &std::get<T>(buffer.back()),
      sizeof(T),
      MPI_BYTE,
      address.GetOutletProc(),
      address.GetTag(),
      address.GetComm(),
      &std::get<uitsl::Request>(buffer.back())
    );

  }

  bool TryFinalizeSend() {
    emp_assert( !uitsl::test_null( std::get<uitsl::Request>(buffer.front()) ) );

    if (uitsl::test_completion( std::get<uitsl::Request>(buffer.front()) )) {
      emp_assert( uitsl::test_null( std::get<uitsl::Request>(buffer.front()) ) );
      buffer.pop_front();
      return true;
    } else return false;

  }

  void CancelPendingSend() {
    emp_assert( !uitsl::test_null( std::get<uitsl::Request>(buffer.front()) ) );

    UIT_Cancel( &std::get<uitsl::Request>(buffer.front()) );
    UIT_Request_free( &std::get<uitsl::Request>(buffer.front()) );

    emp_assert( uitsl::test_null( std::get<uitsl::Request>(buffer.front()) ) );

    buffer.pop_front();
  }

  void FlushFinalizedSends() { while (buffer.size() && TryFinalizeSend()); }

public:

  TrivialDequeImmediateSendDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end
  ) : address(address_)
  { }

  ~TrivialDequeImmediateSendDuct() {
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
      uitsl::Request{}
    );
    emp_assert( uitsl::test_null( std::get<uitsl::Request>(buffer.back()) ) );
    PostSend();
    return true;
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() const { return true; }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    throw "ConsumeGets called on TrivialDequeImmediateSendDuct";
  }

  [[noreturn]] const T& Get() const {
    throw "Get called on TrivialDequeImmediateSendDuct";
  }

  [[noreturn]] T& Get() {
    throw "Get called on TrivialDequeImmediateSendDuct";
  }

  static std::string GetType() { return "TrivialDequeImmediateSendDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << uitsl::format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << uitsl::format_member("InterProcAddress address", address) << std::endl;
    return ss.str();
  }

};

} // namespace internal
} // namespace uit
