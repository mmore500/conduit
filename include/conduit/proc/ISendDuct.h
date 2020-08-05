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

namespace uit {

template<typename T, size_t N>
class Duct;

// TODO rename ProcInletDuct
template<typename T, size_t N=DEFAULT_BUFFER>
class ISendDuct {

  friend Duct<T, N>;

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

  MPI_Comm comm;

  const int outlet_proc;
  const int tag;

  index_t send_position{0};

  void RequestSend() {
    emp_assert(
      request_states[send_position] == false,
      send_position,
      pending,
      format_member("*this", *this)
    );
    verify(MPI_Isend(
      &buffer[send_position],
      sizeof(T),
      MPI_BYTE, // TODO template on T
      outlet_proc,
      tag,
      comm,
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

  ISendDuct(
    const int inlet_proc,
    const int outlet_proc_,
    const int tag_=0,
    MPI_Comm comm_=MPI_COMM_WORLD
  ) : comm(comm_)
  , outlet_proc(outlet_proc_)
  , tag(tag_) {
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

  ~ISendDuct() {
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
    throw "bad Pop on ISendDuct";
  }

  [[noreturn]] size_t GetPending() {
    throw "bad GetPending on ISendDuct";
  }

  size_t GetAvailableCapacity() {
    Flush();
    return N - pending;
  }

  T GetElement(const size_t n) const { return buffer[n]; }

  const void * GetPosition(const size_t n) const { return &buffer[n]; }

  void SetElement(const size_t n, const T & val) { buffer[n] = val; }

  std::string GetType() const { return "ISendDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("pending_t pending", (size_t) pending) << std::endl;
    ss << format_member(
      "MPI_Comm comm",
      [this](){
        int len;
        char data[MPI_MAX_OBJECT_NAME];
        // TODO at least log/warn error codes
        verify(MPI_Comm_get_name(comm, data, &len));
        return std::string{}.assign(data, len);
      }()
    ) << std::endl;
    ss << format_member("get_rank()", get_rank()) << std::endl;
    ss << format_member("int outlet_proc", outlet_proc) << std::endl;
    ss << format_member("int tag", tag) << std::endl;
    ss << format_member("size_t send_position", send_position);
    return ss.str();
  }

};

}
