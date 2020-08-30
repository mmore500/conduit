#pragma once

#include <algorithm>
#include <array>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../distributed/mpi_utils.hpp"
#include "../../../distributed/RDMAWindowManager.hpp"
#include "../../../distributed/Request.hpp"
#include "../../../utility/CircularIndex.hpp"
#include "../../../utility/identity.hpp"
#include "../../../utility/print_utils.hpp"
#include "../../../utility/WarnOnce.hpp"

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
class RputDuct {

public:

  using BackEndImpl = uit::internal::RdmaBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using buffer_t = emp::array<T, N>;
  buffer_t buffer{};

  using index_t = CircularIndex<N>;
  index_t put_position{};

  emp::array<uit::Request, N> put_requests;
  size_t pending_puts{};

  const uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  uit::Request target_offset_request;
  int target_offset;

  /*
   * notes
   *
   * key:
   * - R = open put request
   * - N = put position (where next put request will go)
   * - S = stalest request position
   * initial state:
   * ```
   * : S :
   * : N :
   * |   |   |   |   |   |   |   |   |   |   |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * after one put posted:
   * ```
   * : S : N :
   * | R |   |   |   |   |   |   |   |   |   |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * after another put posted:
   * ```
   * : S :   : N :
   * | R | R |   |   |   |   |   |   |   |   |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * after a put finalized:
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
   * one more put posted:
   * ```
   * : N :
   * : S :
   * | R | R | R | R | R | R | R | R | R | R |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * one put finalized:
   * ```
   * : N : S :
   * |   | R | R | R | R | R | R | R | R | R |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * another put finalized:
   * ```
   * : N :   : S :
   * |   |   | R | R | R | R | R | R | R | R |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   */

  void PostPut() {

    // make sure that target offset has been received
    emp_assert( uit::test_completion(target_offset_request) );

    // TODO handle more than one at a time
    emp_assert( uit::test_null(put_requests[put_position]) );

    back_end->GetWindowManager().LockExclusive( address.GetOutletProc() );

    back_end->GetWindowManager().Rput(
      address.GetOutletProc(),
      &buffer[put_position],
      target_offset,
      &put_requests[put_position]
    );

    back_end->GetWindowManager().Unlock( address.GetOutletProc() );

    emp_assert( !uit::test_null(put_requests[put_position]) );

    ++put_position;
    ++pending_puts;

  }

  index_t CalcStalestPutPos() const { return put_position - pending_puts; }

  bool TryFinalizePut() {
    emp_assert( !uit::test_null(put_requests[CalcStalestPutPos()]) );

    if (uit::test_completion( put_requests[CalcStalestPutPos()] )) {
      --pending_puts;
      emp_assert( uit::test_null(put_requests[CalcStalestPutPos() - 1]) );
      return true;
    } else return false;

  }

  void CancelPendingPut() {
    emp_assert(!uit::test_null( put_requests[CalcStalestPutPos()] ));

    UIT_Cancel( &put_requests[CalcStalestPutPos()] );
    UIT_Request_free( &put_requests[CalcStalestPutPos()] );

    emp_assert(uit::test_null( put_requests[CalcStalestPutPos()] ));

    --pending_puts;
  }

  void FlushFinalizedPuts() { while (pending_puts && TryFinalizePut()); }

public:

  RputDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_
  ) : address(address_)
  , back_end(back_end_)
  {

    emp_assert( std::all_of(
      std::begin(put_requests),
      std::end(put_requests),
      [](const auto& req){ return uit::test_null( req ); }
    ) );

    if (uit::get_rank(address.GetComm()) == address.GetInletProc()) {
      // make spoof call to ensure reciporical activation
      back_end->GetWindowManager().Acquire(address.GetOutletProc(), 0);

      // we'll emp_assert later to make sure it actually completed
      UIT_Irecv(
        &target_offset, // void *buf
        1, // int count
        MPI_INT, // MPI_Datatype datatype
        address.GetOutletProc(), // int source
        address.GetTag(), // int tag
        address.GetComm(), // MPI_Comm comm
        &target_offset_request // MPI_Request *request
      );
    }

    static const uit::WarnOnce warning{
      "RputDuct is experimental and may be unreliable"
    };

  }

  ~RputDuct() {
    FlushFinalizedPuts();
    while (pending_puts) CancelPendingPut();
    emp_assert( std::all_of(
      std::begin(put_requests),
      std::end(put_requests),
      [](const auto& req){ return uit::test_null( req ); }
    ) );
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  void Put(const T& val) {
    emp_assert( pending_puts < N );
    emp_assert( uit::test_null( put_requests[put_position] ) );
    buffer[put_position] = val;
    PostPut();
    emp_assert( pending_puts <= N );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  bool IsReadyForPut() {
    FlushFinalizedPuts();
    return pending_puts < N;
  }

  [[noreturn]] size_t CountUnconsumedGets() const {
    throw "CountUnconsumedGets called on RputDuct";
  }

  [[noreturn]] size_t ConsumeGets(const size_t requested) const {
    throw "ConsumeGets called on RputDuct";
  }

  [[noreturn]] const T& Get() const { throw "Get called on RputDuct"; }

  static std::string GetType() { return "RputDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("size_t pending_puts", pending_puts) << std::endl;
    ss << format_member("InterProcAddress address", address) << std::endl;
    ss << format_member("size_t put_position", put_position);
    return ss.str();
  }

};

} // namespace uit
