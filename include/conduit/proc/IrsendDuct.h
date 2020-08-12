#pragma once

#include "mpi.h"

#include <algorithm>
#include <array>
#include <stddef.h>

#include "base/assert.h"
#include "tools/string_utils.h"

#include "../../utility/CircularIndex.h"
#include "../../utility/identity.h"

#include "../config.h"
#include "../../distributed/mpi_utils.h"
#include "../../utility/print_utils.h"
#include "InterProcAddress.h"
#include "SharedBackEnd.h"

namespace uit {

template<typename ImplSpec>
class Duct;

template<typename ImplSpec>
class IrsendDuct {

  friend Duct<ImplSpec>;

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using pending_t = size_t;
  using buffer_t = emp::array<T, N>;
  using index_t = CircularIndex<N>;

  pending_t pending{0};
  buffer_t buffer;

  emp::array<MPI_Request, N> send_requests;
#ifndef NDEBUG
  // most vexing parse :/
  emp::vector<char> request_states=emp::vector<char>(N, false);
#endif

  const uit::InterProcAddress address;

  index_t send_position{0};

  void RequestSend() {
    emp_assert(
      request_states[send_position] == false,
      send_position,
      pending,
      format_member("*this", *this)
    );
    verify(MPI_Irsend(
      &buffer[send_position],
      sizeof(T),
      MPI_BYTE, // TODO template on T
      address.GetOutletProc(),
      address.GetTag(),
      address.GetComm(),
      &send_requests[send_position]
    ));
#ifndef NDEBUG
    request_states[send_position] = true;
#endif
    ++pending;
    ++send_position;

  }

  // AcceptSend Take
  bool ConfirmSend() {

    int flag{};

    emp_assert(
      request_states[send_position - pending],
      send_position,
      pending,
      format_member("*this", *this)
    );
    verify(MPI_Test(
      &send_requests[send_position - pending],
      &flag,
      MPI_STATUS_IGNORE
    ));
#ifndef NDEBUG
    if(flag) request_states[send_position - pending] = false;
#endif

    if (flag) --pending;

    return flag;

  }

  void CancelSend() {

    emp_assert(
      request_states[send_position - pending],
      send_position,
      pending,
      format_member("*this", *this)
    );
    verify(MPI_Cancel(
      &send_requests[send_position - pending]
    ));
#ifndef NDEBUG
    request_states[send_position - pending] = false;
#endif

    --pending;

  }

  void Flush() { while (pending && ConfirmSend()); }

public:

  IrsendDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<uit::SharedBackEnd<ImplSpec>> back_end
  ) : address(address_)
  {
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

  ~IrsendDuct() {
    while (pending) CancelSend();
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

  //todo rename
  void Push() {

    emp_assert(
      pending < N,
      [](){ error_message_mutex.lock(); return "locked"; }(),
      emp::to_string("pending: ", pending)
    );

    RequestSend();

  }

  void Initialize(const size_t write_position) {
    send_position = write_position;
  }

  //todo rename
  [[noreturn]] void Pop(const size_t count) {
    throw "bad Pop on IrsendDuct";
  }

  [[noreturn]] size_t GetPending() {
    throw "bad GetPending on IrsendDuct";
  }

  size_t GetAvailableCapacity() {
    Flush();
    return N - pending;
  }

  T GetElement(const size_t n) const { return buffer[n]; }

  const void * GetPosition(const size_t n) const { return &buffer[n]; }

  void SetElement(const size_t n, const T & val) { buffer[n] = val; }

  static std::string GetType() { return "IrsendDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("pending_t pending", (size_t) pending) << std::endl;
    ss << format_member("InterProcAddress address", address) << std::endl;
    ss << format_member("size_t send_position", send_position);
    return ss.str();
  }

};

}
