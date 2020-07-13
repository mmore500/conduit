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
class ProcessOutletDuct {

  friend Duct<T, N>;

  using pending_t = size_t;
  using buffer_t = std::array<T, N>;
  using index_t = CircularIndex<N>;

  //TODO fix all these damn mutables
  mutable pending_t pending{0};
  mutable buffer_t buffer;

  mutable std::array<MPI_Request, N> receive_requests;

  MPI_Comm comm;

  const int inlet_proc;
  const int outlet_proc;
  const int tag;

  mutable index_t receive_position{0};

  void RequestReceive() const {
    // TODO add assert to check status of existing request
    verify(MPI_Irecv(
      &buffer[receive_position],
      1,
      MPI_BYTE, // TODO template on T
      inlet_proc,
      tag,
      comm,
      &receive_requests[receive_position]
    ));
    ++receive_position;
  }

  // AcceptReceive Take
  bool ConfirmReceive() const {

    int flag{};

    verify(MPI_Test(
      &receive_requests[receive_position],
      &flag,
      MPI_STATUS_IGNORE
    ));

    if (flag) {
      RequestReceive();
      ++pending;
    }

    return flag;

  }

public:

  ProcessOutletDuct(
    MPI_Comm comm_,
    const int inlet_proc_,
    const int outlet_proc_,
    const int tag_
  ) : comm(comm_)
  , inlet_proc(inlet_proc_)
  , outlet_proc(outlet_proc_)
  , tag(tag_) { for (size_t i = 0; i < N; ++i) RequestReceive(); }

  //todo rename
  void Push() { emp_assert(false); }

  //todo rename
  void Pop(const size_t count) {

    emp_assert(
      pending >= count,
      [](){ error_message_mutex.lock(); return "locked"; }(),
      emp::to_string("pending: ", pending),
      emp::to_string("count: ", count)
    );

    pending -= count;

  }

  size_t GetPending() const {
    while (ConfirmReceive());
    return pending;
  }

  size_t GetAvailableCapacity() const { emp_assert(false); }

  T GetElement(const size_t n) const { return buffer[n]; }

  void SetElement(const size_t n, const T & val) { emp_assert(false); }

  std::string ToString() const {
    std::stringstream ss;
    ss << "ProcessOutletDuct" << std::endl;
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
    ss << format_member("int tag", tag) << std::endl << std::endl;
    ss << format_member("size_t receive_position", receive_position);
    return ss.str();
  }

};
