#pragma once

#include "mpi.h"

#include "base/assert.h"
#include "tools/string_utils.h"

#include "CircularIndex.h"

#include "config_utils.h"
#include "mpi_utils.h"
#include "print_utils.h"

template<typename T, size_t N>
class Duct;

template<typename T, size_t N=DEFAULT_BUFFER>
class ProcessInletDuct {

  friend Duct<T, N>;

  using pending_t = size_t;
  using buffer_t = std::array<T, N>;
  using index_t = CircularIndex<N>;

  //TODO fix all these damn mutables
  mutable pending_t pending{0};
  mutable buffer_t buffer;

  mutable std::array<MPI_Request, N> send_requests;

  MPI_Comm comm;

  const int inlet_proc;
  const int outlet_proc;
  const int tag;

  mutable index_t send_position{0};

  void RequestSend() const {
    // TODO add assert to check status of existing request
    verify(MPI_Isend(
      &buffer[send_position],
      1,
      MPI_BYTE, // TODO template on T
      outlet_proc,
      tag,
      comm,
      &send_requests[send_position]
    ));
    ++pending;
    ++send_position;

  }

  // AcceptSend Take
  bool ConfirmSend() const {

    int flag{};

    verify(MPI_Test(
      &send_requests[send_position - pending],
      &flag,
      MPI_STATUS_IGNORE
    ));

    if (flag) --pending;

    return flag;

  }

public:

  ProcessInletDuct(
    MPI_Comm comm_,
    const int inlet_proc_,
    const int outlet_proc_,
    const int tag_
  ) : comm(comm_)
  , inlet_proc(inlet_proc_)
  , outlet_proc(outlet_proc_)
  , tag(tag_) { ; }

  //todo rename
  void Push() {

    emp_assert(
      pending < N,
      [](){ error_message_mutex.lock(); return "locked"; }(),
      emp::to_string("pending: ", pending)
    );

    RequestSend();

  }

  //todo rename
  void Pop(const size_t count) { emp_assert(false); }

  size_t GetPending() const { emp_assert(false); }

  size_t GetAvailableCapacity() const {
    if (pending) while (ConfirmSend());
    return N - pending;
  }

  T GetElement(const size_t n) const { emp_assert(false); }

  void SetElement(const size_t n, const T & val) { buffer[n] = val; }

  std::string ToString() const {
    std::stringstream ss;
    ss << "ProcessInletDuct" << std::endl;
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
    ss << format_member("int inlet_proc", inlet_proc) << std::endl;
    ss << format_member("int outlet_proc", outlet_proc) << std::endl;
    ss << format_member("int tag", tag) << std::endl;
    ss << format_member("size_t send_position", send_position);
    return ss.str();
  }

};
