#pragma once

#include "mpi.h"

#include <algorithm>
#include <array>
#include <stddef.h>

#include "base/assert.h"
#include "tools/string_utils.h"

#include "../../distributed/RDMAWindowManager.h"
#include "../../utility/CircularIndex.h"
#include "../../utility/identity.h"

#include "../config.h"
#include "../../distributed/mpi_utils.h"
#include "../../utility/print_utils.h"

namespace uit {

template<typename T, size_t N>
class Duct;

// TODO rename RputDuct
template<typename T, size_t N=DEFAULT_BUFFER>
class RputDuct {

  friend Duct<T, N>;

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

  MPI_Comm comm;

  const int outlet_proc;

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

    RDMAWindowManager::LockExclusive(outlet_proc);

    RDMAWindowManager::Rput(
      outlet_proc,
      &buffer[send_position],
      target_offset,
      &send_requests[send_position]
    );

    RDMAWindowManager::Unlock(outlet_proc);

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
    if (flag) request_states[send_position] = false;
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
    const int inlet_proc,
    const int outlet_proc_=MPI_ANY_SOURCE,
    const int tag_=0,
    MPI_Comm comm_=MPI_COMM_WORLD
  ) : comm(comm_)
  , outlet_proc(outlet_proc_) {

    // make spoof call to ensure reciporical activation
    RDMAWindowManager::Acquire(outlet_proc, 0);

    if (get_rank(comm) == inlet_proc) {
      // we'll emp_assert to make sure it actually completed
      verify(MPI_Irecv(
        &target_offset, // void *buf
        1, // int count
        MPI_INT, // MPI_Datatype datatype
        outlet_proc, // int source
        tag_, // int tag
        comm, // MPI_Comm comm
        &target_offset_request // MPI_Request *request
      ));
    }

    emp_assert(
      true,
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

  std::string GetType() const { return "RputDuct"; }

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
    ss << format_member("size_t send_position", send_position);
    return ss.str();
  }

};

}
