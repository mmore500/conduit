#pragma once

#include <mpi.h>

#include <algorithm>
#include <array>
#include <stddef.h>

#include "../../../third-party/Empirical/source/base/assert.h"
#include "../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../distributed/RDMAWindowManager.hpp"
#include "../../utility/CircularIndex.hpp"
#include "../../utility/identity.hpp"

#include "../config.hpp"
#include "../../distributed/mpi_utils.hpp"
#include "../../utility/print_utils.hpp"
#include "InterProcAddress.hpp"
#include "SharedBackEnd.hpp"

namespace uit {

template<typename ImplSpec>
class Duct;

// TODO rename RputDuct
template<typename ImplSpec>
class RputDuct {

  friend Duct<ImplSpec>;

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using pending_t = size_t;
  using buffer_t = emp::array<T, N>;
  using index_t = CircularIndex<N>;

  pending_t pending{0};
  buffer_t buffer;
  //TODO do we need a buffer if we're just overwriting on the other end?

  emp::array<MPI_Request, N> send_requests;
#ifndef NDEBUG
  // most vexing parse :/
  emp::vector<char> request_states=emp::vector<char>(N, false);
#endif

  const uit::InterProcAddress address;
  std::shared_ptr<uit::SharedBackEnd<ImplSpec>> back_end;

  MPI_Request target_offset_request;
  int target_offset;

  index_t send_position{0};

  void RequestSend() {

    // make sure that target offset has been received
    emp_assert([this](){
      int flag{};
      verify(MPI_Test(
        &target_offset_request,
        &flag,
        MPI_STATUS_IGNORE
      ));
      return flag;
    }());

    // TODO handle more than one at a time
    emp_assert(
      request_states[send_position] == false,
      send_position,
      pending,
      format_member("*this", *this)
    );

    back_end->GetWindowManager().LockExclusive(address.GetOutletProc());

    back_end->GetWindowManager().Rput(
      address.GetOutletProc(),
      &buffer[send_position],
      target_offset,
      &send_requests[send_position]
    );

    back_end->GetWindowManager().Unlock(address.GetOutletProc());

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
    if (flag) request_states[send_position - pending] = false;
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

  // TODO check if is inlet proc
  RputDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<uit::SharedBackEnd<ImplSpec>> back_end_
  ) : address(address_)
  , back_end(back_end_)
  {

    if (uit::get_rank(address.GetComm()) == address.GetInletProc()) {
      // make spoof call to ensure reciporical activation
      back_end->GetWindowManager().Acquire(address.GetOutletProc(), 0);

      // we'll emp_assert later to make sure it actually completed
      verify(MPI_Irecv(
        &target_offset, // void *buf
        1, // int count
        MPI_INT, // MPI_Datatype datatype
        address.GetOutletProc(), // int source
        address.GetTag(), // int tag
        address.GetComm(), // MPI_Comm comm
        &target_offset_request // MPI_Request *request
      ));
    }

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

  ~RputDuct() {
    Flush();
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
    throw "bad Pop on RputDuct";
  }

  [[noreturn]] size_t GetPending() {
    throw "bad GetPending on RputDuct";
  }

  size_t GetAvailableCapacity() {
    Flush();
    return N - pending;
  }

  T GetElement(const size_t n) const { return buffer[n]; }

  const void * GetPosition(const size_t n) const { return &buffer[n]; }

  void SetElement(const size_t n, const T & val) { buffer[n] = val; }

  static std::string GetType() { return "RputDuct"; }

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
