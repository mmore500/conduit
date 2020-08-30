#pragma once

#include <algorithm>
#include <array>
#include <stddef.h>

#include <mpi.h>

#include "../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../distributed/mpi_utils.hpp"
#include "../../../distributed/Request.hpp"
#include "../../../utility/CircularIndex.hpp"
#include "../../../utility/identity.hpp"
#include "../../../utility/print_utils.hpp"

#include "../../InterProcAddress.hpp"

#include "../backend/MockBackEnd.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class IrecvDuct {

public:

  using BackEndImpl = uit::internal::MockBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  size_t pending_gets{};

  using buffer_t = emp::array<T, N>;
  buffer_t buffer{};

  emp::array<uit::Request, N> receive_requests;

  // skip first to allow for default-constructed initial get
  using index_t = uit::CircularIndex<N>;
  index_t freshest_receive_pos{};

  const uit::InterProcAddress address;


  /*
   * notes
   *
   * key:
   * - R = open receive request
   * - P = pending get
   *
   * - G = current get position
   * - S = stalest receive position
   * - F = freshest receive position (1 before where next Request will go)
   *
   * initial state:
   * ```
   * : G : S :                           : F :
   * |   | R | R | R | R | R | R | R | R | R |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * one receive fulfilled:
   * ```
   * : G :   : S :                       : F :
   * |   | P | R | R | R | R | R | R | R | R |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * another receive fulfilled:
   * ```
   * : G :       : S :                   : F :
   * |   | P | P | R | R | R | R | R | R | R |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * one get consumed:
   * ```
   * : F : G :   : S :
   * | R |   | P | R | R | R | R | R | R | R |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * another get consumed:
   * ```
   *     : F : G : S :
   * | R | R |   | R | R | R | R | R | R | R |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * possible almost maxed-out pending gets state:
   * ```
   *                                     : S :
   * : G :                               : F :
   * |   | P | P | P | P | P | P | P | P | R |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * another possible almost maxed-out pending gets state:
   * ```
   * : S :
   * : F : G :
   * | R |   | P | P | P | P | P | P | P | P |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * possible maxed-out pending gets state:
   * ```
   * : S :
   * : G :                               : F :
   * |   | P | P | P | P | P | P | P | P | P |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * another possible maxed-out pending gets state:
   * ```
   *     : S :
   * : F : G :
   * | P |   | P | P | P | P | P | P | P | P |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   *
   * want to test requests from `S` to `F`
   * 1. no requests available
   *  - true if `pending_gets == N - 1`
   *  - return 0
   * 2. contiguous requests case
   *  - true if `S <= F` and requests are available
   *  - test from `S` to `F + 1` (non-inclusive)
   * 3. wraparound requests case
   *  - true if `F > S` and requests are available
   *  - test from `S` to `N` (non-inclusive)
   *  - if `result == N - S` then test from `0 to F + 1` (non-inclusive)
   */

  index_t CalcGetPosition() const { return freshest_receive_pos + 1; }

  index_t CalcStalestReceivePos() const {
    return CalcGetPosition() + pending_gets + 1;
  }

  bool ReceiveRequestsAreContiguous() const {
    emp_assert( pending_gets < N - 1 );
    return CalcStalestReceivePos() <= freshest_receive_pos;
  }

  void PostReceiveRequest() {
    ++freshest_receive_pos;

    emp_assert( uit::test_null( receive_requests[freshest_receive_pos] ) );
    uit::verify(MPI_Irecv(
      &buffer[freshest_receive_pos],
      sizeof(T),
      MPI_BYTE,
      address.GetInletProc(),
      address.GetTag(),
      address.GetComm(),
      &receive_requests[freshest_receive_pos]
    ));
    emp_assert( !uit::test_null( receive_requests[freshest_receive_pos] ) );

  }

  void CancelReceiveRequest(const size_t pos) {
    emp_assert( !uit::test_null( receive_requests[pos] ) );

    uit::verify(MPI_Cancel( &receive_requests[pos] ));
    uit::verify(MPI_Request_free( &receive_requests[pos] ));

    emp_assert( uit::test_null( receive_requests[pos] ) );
  }

  void CancelOpenReceiveRequests() {
    for (
      index_t idx = CalcStalestReceivePos();
      idx != CalcGetPosition();
      ++idx
    ) CancelReceiveRequest(idx);
  }

  // to is non-inclusive
  size_t TestRequests(const size_t from, const size_t to, const int context) {
    emp_assert(from <= to);
    emp_assert(to - from < N);
    emp_assert( std::none_of(
      std::begin(receive_requests) + from,
      std::begin(receive_requests) + to,
      [](const auto& req){ return uit::test_null( req ); }
    ) );

    thread_local emp::array<int, N> out_indices; // ignored
    int received_count{};

    uit::verify(MPI_Testsome(
      to - from, // int count
      &receive_requests[from], // MPI_Request array_of_requests[]
      &received_count, // int *outcount
      out_indices.data(), // int *indices
      MPI_STATUSES_IGNORE // MPI_Status array_of_statuses[]
    ));

    emp_assert( std::all_of(
      std::begin(receive_requests) + from,
      std::begin(receive_requests) + received_count,
      [](const auto& req){ return uit::test_null( req ); }
    ) );

    return received_count;
  }

  // returns number of requests fulfilled
  void TryFulfillReceiveRequests() {
    pending_gets += [this]() -> size_t {

      if ( pending_gets == N - 1 ) {
        return 0;
      } else if ( ReceiveRequestsAreContiguous() ) {
        return TestRequests(
          CalcStalestReceivePos(),
          static_cast<size_t>(freshest_receive_pos) + 1,
          0
        );
      } else {
        size_t res = TestRequests( CalcStalestReceivePos(), N, 1 );

        if ( res == N - CalcStalestReceivePos() ) res += TestRequests(
          0,
          static_cast<size_t>(freshest_receive_pos) + 1,
          2
        );

        return res;
      }

    }();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t CountUnconsumedGets() {
    TryFulfillReceiveRequests();
    return pending_gets;
  }

public:

  IrecvDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end
  ) : address(address_) {

    emp_assert( std::all_of(
      std::begin(buffer),
      std::end(buffer),
      [](const auto& val){ return val == T{}; }
    ) );

    for (size_t i = 1; i < N; ++i) PostReceiveRequest();
    emp_assert( std::none_of(
      std::next(std::begin(receive_requests)),
      std::end(receive_requests),
      [](const auto& req){ return uit::test_null( req ); }
    ) );
  }

  ~IrecvDuct() {
    while ( CountUnconsumedGets() ) TryConsumeGets( CountUnconsumedGets() );
    CancelOpenReceiveRequests();
    emp_assert( std::all_of(
      std::begin(receive_requests),
      std::end(receive_requests),
      [](const auto& req){ return uit::test_null( req ); }
    ) );
  }

  [[noreturn]] void Put(const T& val) const { throw "Put called on IrecvDuct"; }

  [[noreturn]] bool IsReadyForPut() const {
    throw "IsReadyForPut called on IrecvDuct";
  }

  /**
   * TODO.
   *
   * @param num_requested TODO.
   * @return number items consumed.
   */
  size_t TryConsumeGets(const size_t num_requested) {

    size_t requested_countdown{ num_requested };
    size_t batch_countdown{ CountUnconsumedGets() };
    bool full_batch = (batch_countdown == N - 1);

    while ( batch_countdown && requested_countdown ) {

      --batch_countdown;
      --requested_countdown;
      --pending_gets;
      PostReceiveRequest();

      if (full_batch && batch_countdown == 0) {
        batch_countdown = CountUnconsumedGets();
        full_batch = (batch_countdown == N - 1);
      }
    }

    return num_requested - requested_countdown;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return buffer[CalcGetPosition()]; }

  static std::string GetName() { return "IrecvDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("size_t pending_gets", pending_gets) << std::endl;
    ss << format_member("InterProcAddress address", address) << std::endl;
    ss << format_member("size_t freshest_receive_pos", freshest_receive_pos);
    return ss.str();
  }

};

} // namespace uit
