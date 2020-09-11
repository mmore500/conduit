#pragma once

#include <algorithm>
#include <array>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../mpi/mpi_utils.hpp"
#include "../../../../mpi/Request.hpp"
#include "../../../../utility/CircularIndex.hpp"
#include "../../../../utility/identity.hpp"
#include "../../../../utility/print_utils.hpp"

#include "../../../InterProcAddress.hpp"

#include "../../backend/MockBackEnd.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class BlockIrecvDuct {

public:

  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using buffer_t = emp::array<T, N>;
  buffer_t buffer{};

  emp::array<uit::Request, N> receive_requests;

  using index_t = uit::CircularIndex<N>;
  index_t receive_position{1};

  const uit::InterProcAddress address;

  void PostReceiveRequest() {
    emp_assert( uit::test_null( receive_requests[receive_position] ) );
    UIT_Irecv(
      &buffer[receive_position],
      sizeof(T),
      MPI_BYTE,
      address.GetInletProc(),
      address.GetTag(),
      address.GetComm(),
      &receive_requests[receive_position]
    );
    emp_assert( !uit::test_null( receive_requests[receive_position] ) );
    ++receive_position;
  }

  void CancelReceiveRequest(const size_t pos) {
    emp_assert( !uit::test_null( receive_requests[pos] ) );

    UIT_Cancel( &receive_requests[pos] );
    UIT_Request_free( &receive_requests[pos] );

    emp_assert( uit::test_null( receive_requests[pos] ) );
  }

  void CancelAllReceiveRequests() {
    for (size_t idx = 0; idx < N; ++idx) CancelReceiveRequest(idx);
  }

  // returns true if block was full
  size_t FlushBlock() {

    emp_assert( std::none_of(
      std::begin(receive_requests),
      std::end(receive_requests),
      [](const auto& req){ return uit::test_null( req ); }
    ) );

    int count{};
    thread_local emp::array<int, N> out_indices; // ignored

    UIT_Testsome(
      N, // int count
      &receive_requests[0], // MPI_Request array_of_requests[]
      &count, // int *outcount
      out_indices.data(), // int *indices
      MPI_STATUSES_IGNORE // MPI_Status array_of_statuses[]
    );
    for (int i = 0; i < count; ++i) PostReceiveRequest();

    emp_assert( std::none_of(
      std::begin(receive_requests),
      std::end(receive_requests),
      [](const auto& req){ return uit::test_null( req ); }
    ) );

    emp_assert(count >= 0);

    return count;

  }

  size_t FlushReceives() {
    size_t num_flushed;
    size_t total_flushed{};
    do {
      num_flushed = FlushBlock();
      total_flushed += num_flushed;
    } while (num_flushed == N);
    return total_flushed;
  }

public:

  BlockIrecvDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end
  ) : address(address_) {

    emp_assert( std::all_of(
      std::begin(buffer),
      std::end(buffer),
      [](const auto& val){ return val == T{}; }
    ) );

    for (size_t i = 0; i < N; ++i) PostReceiveRequest();
    emp_assert( std::none_of(
      std::begin(receive_requests),
      std::end(receive_requests),
      [](const auto& req){ return uit::test_null( req ); }
    ) );
  }

  ~BlockIrecvDuct() {
    FlushReceives();
    CancelAllReceiveRequests();
    emp_assert( std::all_of(
      std::begin(receive_requests),
      std::end(receive_requests),
      [](const auto& req){ return uit::test_null( req ); }
    ) );
  }

  [[noreturn]] bool TryPut(const T&) const {
    throw "TryPut called on BlockIrecvDuct";
  }

  [[noreturn]] void Flush() const { throw "Flush called on BlockIrecvDuct"; }

  /**
   * TODO.
   *
   * @param num_requested TODO.
   * @return number items consumed.
   */
  size_t TryConsumeGets(const size_t num_requested) {

    emp_assert( num_requested == std::numeric_limits<size_t>::max() );

    return FlushReceives();

  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const {
    //TODO tidy up
    const uit::CircularIndex<N> idx = receive_position - 1;
    return buffer[idx];
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() {
    //TODO tidy up
    const uit::CircularIndex<N> idx = receive_position - 1;
    return buffer[idx];
  }

  static std::string GetName() { return "BlockIrecvDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("InterProcAddress address", address) << std::endl;
    ss << format_member("size_t receive_position", receive_position);
    return ss.str();
  }

};

} // namespace uit
