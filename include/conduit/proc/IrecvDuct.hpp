#pragma once

#include <algorithm>
#include <array>
#include <stddef.h>

#include <mpi.h>

#include "../../../third-party/Empirical/source/base/assert.h"
#include "../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../distributed/mpi_utils.hpp"
#include "../../utility/CircularIndex.hpp"
#include "../../utility/identity.hpp"
#include "../../utility/print_utils.hpp"

#include "../config.hpp"

#include "InterProcAddress.hpp"
#include "SharedBackEnd.hpp"

namespace uit {

template<typename ImplSpec>
class Duct;

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class IrecvDuct {

  friend Duct<ImplSpec>;

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using pending_t = size_t;
  using buffer_t = emp::array<T, N>;
  using index_t = CircularIndex<N>;

  pending_t pending{0};
  buffer_t buffer;

  emp::array<MPI_Request, N> receive_requests;
#ifndef NDEBUG
  // most vexing parse :/
  emp::vector<char> request_states=emp::vector<char>(N, false);
#endif

  const uit::InterProcAddress address;

  index_t receive_position{0};

  void RequestReceive() {
    emp_assert(
      request_states[receive_position] == false,
      receive_position,
      pending
    );
    verify(MPI_Irecv(
      &buffer[receive_position],
      sizeof(T),
      MPI_BYTE, // TODO template on T
      address.GetInletProc(),
      address.GetTag(),
      address.GetComm(),
      &receive_requests[receive_position]
    ));
#ifndef NDEBUG
    request_states[receive_position] = true;
#endif
    ++receive_position;
  }

  // AcceptReceive Take
  void ConfirmReceive() {

    emp_assert(
      request_states[receive_position],
      [](){ error_message_mutex.lock(); return "locked"; }(),
      receive_position,
      pending,
      format_member("*this", *this)
    );
#ifndef NDEBUG
    request_states[receive_position] = false;
#endif

    RequestReceive();
    ++pending;

  }

  void CancelReceive(const size_t position) {

    emp_assert(
      request_states[position],
      [](){ error_message_mutex.lock(); return "locked"; }(),
      position,
      receive_position,
      pending,
      format_member("*this", *this)
    );

    verify(MPI_Cancel(
      &receive_requests[position]
    ));
#ifndef NDEBUG
    request_states[position] = false;
#endif

  }

  void CancelReceives() {

    for (size_t position = 0; position < N; ++position) {
      CancelReceive(position);
    }

  }

  void Flush() {
    int count{};
    do {
      thread_local emp::array<int, N> out_indices;

      MPI_Testsome(
        N, // int count
        receive_requests.data(), // MPI_Request array_of_requests[]
        &count, // int *outcount
        out_indices.data(), // int *indices
        MPI_STATUSES_IGNORE // MPI_Status array_of_statuses[]
      );
      for (int i = 0; i < count; ++i) ConfirmReceive();

    } while (count == N);
  }

public:

  IrecvDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<uit::SharedBackEnd<ImplSpec>> back_end
  ) : address(address_) {
    for (size_t i = 0; i < N; ++i) RequestReceive();
    emp_assert(
      std::all_of(
        std::begin(request_states),
        std::end(request_states),
        identity
      ),
      [](){ error_message_mutex.lock(); return "locked"; }(),
      format_member("*this", *this)
    );
  }

  ~IrecvDuct() {
    Flush();
    CancelReceives();
    emp_assert(
      std::none_of(
        std::begin(request_states),
        std::end(request_states),
        identity
      ),
      [](){ error_message_mutex.lock(); return "locked"; }(),
      format_member("*this", *this)
    );
  }

  void Initialize(const size_t write_position) {
    for (size_t i = 0; i < write_position; ++i) {
      emp_assert(
        request_states[receive_position],
        [](){ error_message_mutex.lock(); return "locked"; }(),
        receive_position,
        format_member("*this", *this)
      );

      verify(MPI_Request_free(&receive_requests[i]));

#ifndef NDEBUG
      request_states[receive_position] = false;
#endif
      RequestReceive();
    }
  }

  //todo rename
  [[noreturn]] void Push() { throw "bad Push on IrecvDuct"; }

  //todo rename
  void Pop(const size_t count) {

    emp_assert(
      pending >= count,
      [](){ error_message_mutex.lock(); return "locked"; }(),
      pending,
      count,
      format_member("*this", *this)
    );

    pending -= count;

  }

  size_t GetPending() {
    Flush();
    return pending;
  }

  [[noreturn]] size_t GetAvailableCapacity() {
    throw "bad GetAvailableCapacity on IrecvDuct";
  }

  T GetElement(const size_t n) const { return buffer[n]; }

  const void * GetPosition(const size_t n) const { return &buffer[n]; }

  [[noreturn]] void SetElement(const size_t n, const T & val) {
    throw "bad SetElement on IrecvDuct";
  }

  static std::string GetType() { return "IrecvDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("pending_t pending", (size_t) pending) << std::endl;
    ss << format_member("InterProcAddress address", address) << std::endl;
    ss << format_member("size_t receive_position", receive_position);
    return ss.str();
  }

};

}
