#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../debug/err_audit.hpp"
#include "../../../../mpi/mpi_utils.hpp"
#include "../../../../utility/MirroredRingBuffer.hpp"
#include "../../../../utility/print_utils.hpp"
#include "../../../../utility/RingBuffer.hpp"

#include "../../../InterProcAddress.hpp"

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
class RingIrecvDuct {

public:

  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  // one extra in the data buffer to hold the current get
  uit::RingBuffer<T, N + 1> data;
  // TODO a ring buffer where requests are copied to be contiguous
  // might be more efficient w.r.t. caching
  uit::MirroredRingBuffer<MPI_Request, N> requests;

  const uit::InterProcAddress address;

  void PostReceiveRequest() {
    uit::err_audit(!
      data.PushHead()
    );
    uit::err_audit(!
      requests.PushHead( MPI_REQUEST_NULL )
    );

    emp_assert( uit::test_null( requests.GetHead() ) );
    UIT_Irecv(
      &data.GetHead(),
      sizeof(T),
      MPI_BYTE,
      address.GetInletProc(),
      address.GetTag(),
      address.GetComm(),
      requests.GetHeadPtr()
    );
    emp_assert( !uit::test_null( requests.GetHead() ) );

  }

  void CancelReceiveRequest() {
    emp_assert( !uit::test_null( requests.GetTail() ) );

    UIT_Cancel(  requests.GetTailPtr() );
    UIT_Request_free( requests.GetTailPtr() );

    emp_assert( uit::test_null( requests.GetTail() ) );

    uit::err_audit(!  data.PopTail()  );
    uit::err_audit(!  requests.PopTail()  );

  }

  void CancelOpenReceiveRequests() {
    while ( requests.GetSize() ) CancelReceiveRequest();
  }

  // returns number of requests fulfilled
  size_t TestRequests() {

    // MPICH Testsome returns negative outcount for zero count calls
    // so let's boogie out early to avoid drama
    if (requests.GetSize() == 0) return 0;

    emp_assert( std::none_of(
      requests.GetTailPtr(),
      requests.GetPastHeadPtr(),
      [](const auto& req){ return uit::test_null( req ); }
    ) );

    thread_local emp::array<int, N> out_indices; // ignored
    int num_received{};

    UIT_Testsome(
      requests.GetSize(), // int count
      requests.GetTailPtr(), // MPI_Request array_of_requests[]
      &num_received, // int *outcount
      out_indices.data(), // int *indices
      MPI_STATUSES_IGNORE // MPI_Status array_of_statuses[]
    );

    emp_assert( num_received >= 0 );
    emp_assert( static_cast<size_t>(num_received) <= requests.GetSize() );

    emp_assert( std::all_of(
      requests.GetTailPtr(),
      requests.GetTailPtr() + num_received,
      [](const auto& req){ return uit::test_null( req ); }
    ) );
    emp_assert( std::none_of(
      requests.GetTailPtr() + num_received,
      requests.GetPastHeadPtr(),
      [](const auto& req){ return uit::test_null( req ); }
    ) );

    return num_received;
  }

  void TryFulfillReceiveRequests() {
    const size_t num_received = TestRequests();
    const size_t res = requests.PopTail(num_received);
    emp_assert( res == num_received );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t CountUnconsumedGets() {
    TryFulfillReceiveRequests();
    return data.GetSize() - requests.GetSize() - 1;
  }

public:

  RingIrecvDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end
  ) : address(address_) {

    data.PushHead( T{} ); // value-initialized initial Get item
    for (size_t i = 0; i < N; ++i) PostReceiveRequest();
    emp_assert( std::none_of(
      requests.GetTailPtr(),
      requests.GetPastHeadPtr(),
      [](const auto& req){ return uit::test_null( req ); }
    ) );
  }

  ~RingIrecvDuct() {
    while ( CountUnconsumedGets() ) TryConsumeGets( CountUnconsumedGets() );
    CancelOpenReceiveRequests();
  }

  [[noreturn]] bool TryPut(const T&) const {
    throw "TryPut called on RingIrecvDuct";
  }

  /**
   * TODO.
   *
   */
  [[noreturn]] bool TryFlush() const { throw "Flush called on RingIrecvDuct"; }

  /**
   * TODO.
   *
   * @param num_requested TODO.
   * @return number items consumed.
   */
  size_t TryConsumeGets(const size_t num_requested) {

    size_t requested_countdown{ num_requested };
    size_t batch_countdown{ CountUnconsumedGets() };
    bool full_batch = (batch_countdown == N);

    while ( batch_countdown && requested_countdown ) {

      --batch_countdown;
      --requested_countdown;
      uit::err_audit(!   data.PopTail()   );
      PostReceiveRequest();

      if (full_batch && batch_countdown == 0) {
        batch_countdown = CountUnconsumedGets();
        full_batch = (batch_countdown == N);
      }
    }

    const size_t num_consumed = num_requested - requested_countdown;
    return num_consumed;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return data.GetTail(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { return data.GetTail(); }

  static std::string GetName() { return "RingIrecvDuct"; }

  static constexpr bool CanStep() { return true; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("InterProcAddress address", address) << std::endl;
    return ss.str();
  }

};

} // namespace t
} // namespace uit
