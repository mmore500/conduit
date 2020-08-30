#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include <stddef.h>

#include <mpi.h>

#include "../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../distributed/mpi_utils.hpp"
#include "../../distributed/Request.hpp"
#include "../../utility/CircularIndex.hpp"
#include "../../utility/identity.hpp"
#include "../../utility/print_utils.hpp"

#include "../InterProcAddress.hpp"
#include "../proc/backend/MockBackEnd.hpp"

namespace uit {
namespace internal {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImmediateSendFunctor, typename ImplSpec>
class ImmediateSendDuct {

public:

  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using buffer_t = emp::array<T, N>;
  buffer_t buffer{};

  // where will the next send take place from?
  using index_t = uit::CircularIndex<N>;
  index_t send_position{};

  emp::array<uit::Request, N> send_requests;
  size_t pending_sends{};

  const uit::InterProcAddress address;

  /*
   * notes
   *
   * key:
   * - R = open send request
   * - N = send position (where next send request will go)
   * - S = stalest request position
   * initial state:
   * ```
   * : S :
   * : N :
   * |   |   |   |   |   |   |   |   |   |   |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * after one send posted:
   * ```
   * : S : N :
   * | R |   |   |   |   |   |   |   |   |   |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * after another send posted:
   * ```
   * : S :   : N :
   * | R | R |   |   |   |   |   |   |   |   |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * after a send finalized:
   * ```
   *     : S : N :
   * |   | R |   |   |   |   |   |   |   |   |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * possible almost-full buffer arrangement:
   * ```
   * : S :                               : N :
   * | R | R | R | R | R | R | R | R | R |   |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * one more send posted:
   * ```
   * : N :
   * : S :
   * | R | R | R | R | R | R | R | R | R | R |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * one send finalized:
   * ```
   * : N : S :
   * |   | R | R | R | R | R | R | R | R | R |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * another send finalized:
   * ```
   * : N :   : S :
   * |   |   | R | R | R | R | R | R | R | R |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   */

  void PostSend() {
    emp_assert( uit::test_null(send_requests[send_position]) );

    ImmediateSendFunctor{}(
      &buffer[send_position],
      sizeof(T),
      MPI_BYTE,
      address.GetOutletProc(),
      address.GetTag(),
      address.GetComm(),
      &send_requests[send_position]
    );

    ++send_position;
    ++pending_sends;

  }

  index_t CalcStalestSendPos() const { return send_position - pending_sends; }

  bool TryFinalizeSend() {
    emp_assert( !uit::test_null(send_requests[CalcStalestSendPos()]) );

    if (uit::test_completion( send_requests[CalcStalestSendPos()] )) {
      --pending_sends;
      emp_assert( uit::test_null(send_requests[CalcStalestSendPos() - 1]) );
      return true;
    } else return false;

  }

  void CancelPendingSend() {
    emp_assert(!uit::test_null( send_requests[CalcStalestSendPos()] ));

    UIT_Cancel( &send_requests[CalcStalestSendPos()] );
    UIT_Request_free( &send_requests[CalcStalestSendPos()] );

    emp_assert( uit::test_null( send_requests[CalcStalestSendPos()] ));

    --pending_sends;
  }

  void FlushFinalizedSends() { while (pending_sends && TryFinalizeSend()); }

public:

  ImmediateSendDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end
  ) : address(address_)
  {
    emp_assert( std::all_of(
      std::begin(send_requests),
      std::end(send_requests),
      [](const auto& req){ return uit::test_null( req ); }
    ) );
  }

  ~ImmediateSendDuct() {
    FlushFinalizedSends();
    while (pending_sends) CancelPendingSend();
    emp_assert( std::all_of(
      std::begin(send_requests),
      std::end(send_requests),
      [](const auto& req){ return uit::test_null( req ); }
    ) );
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  void Put(const T& val) {
    emp_assert( pending_sends < N );
    emp_assert( uit::test_null( send_requests[send_position] ) );
    buffer[send_position] = val;
    PostSend();
    emp_assert( pending_sends <= N );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  bool IsReadyForPut() {
    FlushFinalizedSends();
    return pending_sends < N;
  }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    throw "ConsumeGets called on ImmediateSendDuct";
  }

  [[noreturn]] const T& Get() const { throw "Get called on ImmediateSendDuct"; }

  static std::string GetType() { return "ImmediateSendDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("size_t pending_sends", pending_sends) << std::endl;
    ss << format_member("InterProcAddress address", address) << std::endl;
    ss << format_member("size_t send_position", send_position);
    return ss.str();
  }

};

} // namespace internal
} // namespace uit
