#pragma once

#include <algorithm>
#include <array>
#include <memory>
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
class RingImmediateSendDuct {

public:

  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using buffer_t = emp::array<std::tuple<T, uit::Request>, N>;
  buffer_t buffer{};

  // where will the next send take place from?
  using index_t = uit::CircularIndex<N>;
  index_t send_position{};

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
    emp_assert( uit::test_null(std::get<uit::Request>(buffer[send_position])) );

    ImmediateSendFunctor{}(
      &std::get<T>(buffer[send_position]),
      sizeof(T),
      MPI_BYTE,
      address.GetOutletProc(),
      address.GetTag(),
      address.GetComm(),
      &std::get<uit::Request>(buffer[send_position])
    );

    emp_assert(!uit::test_null(std::get<uit::Request>(buffer[send_position])));

    ++send_position;
    ++pending_sends;

  }

  index_t CalcStalestSendPos() const { return send_position - pending_sends; }

  bool TryFinalizeSend() {
    emp_assert(!uit::test_null(
      std::get<uit::Request>(buffer[CalcStalestSendPos()])
    ));

    if (uit::test_completion(
      std::get<uit::Request>(buffer[CalcStalestSendPos()])
    )) {
      --pending_sends;
      emp_assert( uit::test_null(
        std::get<uit::Request>(buffer[CalcStalestSendPos() - 1])
      ) );
      return true;
    } else return false;

  }

  void CancelPendingSend() {
    emp_assert(!uit::test_null(
      std::get<uit::Request>(buffer[CalcStalestSendPos()])
    ));

    UIT_Cancel( &std::get<uit::Request>(buffer[CalcStalestSendPos()]) );
    UIT_Request_free( &std::get<uit::Request>(buffer[CalcStalestSendPos()]) );

    emp_assert( uit::test_null(
      std::get<uit::Request>(buffer[CalcStalestSendPos()])
    ));

    --pending_sends;
  }

  void FlushFinalizedSends() { while (pending_sends && TryFinalizeSend()); }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  void DoPut(const T& val) {
    emp_assert( pending_sends < N );
    emp_assert( uit::test_null(std::get<uit::Request>(buffer[send_position])) );
    std::get<T>(buffer[send_position]) = val;
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

public:

  RingImmediateSendDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end
  ) : address(address_)
  {
    emp_assert( std::all_of(
      std::begin(buffer),
      std::end(buffer),
      [](const auto& tup){ return uit::test_null(std::get<uit::Request>(tup)); }
    ) );
  }

  ~RingImmediateSendDuct() {
    FlushFinalizedSends();
    while (pending_sends) CancelPendingSend();
    emp_assert( std::all_of(
      std::begin(buffer),
      std::end(buffer),
      [](const auto& tup){ return uit::test_null(std::get<uit::Request>(tup)); }
    ) );
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    if (IsReadyForPut()) { DoPut(val); return true; }
    else return false;
  }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    throw "ConsumeGets called on RingImmediateSendDuct";
  }

  [[noreturn]] const T& Get() const {
    throw "Get called on RingImmediateSendDuct";
  }

  [[noreturn]] T& Get() {
    throw "Get called on RingImmediateSendDuct";
  }

  static std::string GetType() { return "RingImmediateSendDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("size_t pending_sends", pending_sends) << std::endl;
    ss << format_member("InterProcAddress address", address) << std::endl;
    ss << format_member("size_t send_position", send_position);
    return ss.str();
  }

};

} // namespace internal
} // namespace uit
