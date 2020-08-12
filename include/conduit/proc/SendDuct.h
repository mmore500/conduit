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
class SendDuct {

  friend Duct<ImplSpec>;

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using buffer_t = emp::array<T, N>;
  using index_t = CircularIndex<N>;

  buffer_t buffer;

  const uit::InterProcAddress address;

  index_t send_position{0};

  void DoSend() {
    verify(MPI_Send(
      &buffer[send_position],
      sizeof(T),
      MPI_BYTE, // TODO template on T
      address.GetOutletProc(),
      address.GetTag(),
      address.GetComm()
    ));
    ++send_position;
  }

public:

  SendDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<uit::SharedBackEnd<ImplSpec>> back_end
  ) : address(address_)
  { ; }

  ~SendDuct() {
  }

  //todo rename
  void Push() { DoSend(); }

  void Initialize(const size_t write_position) {
    send_position = write_position;
  }

  //todo rename
  [[noreturn]] void Pop(const size_t count) { throw "bad Pop on SendDuct"; }

  [[noreturn]] size_t GetPending() { throw "bad GetPending on SendDuct"; }

  size_t GetAvailableCapacity() { return N; }

  T GetElement(const size_t n) const { return buffer[n]; }

  const void * GetPosition(const size_t n) const { return &buffer[n]; }

  void SetElement(const size_t n, const T & val) { buffer[n] = val; }

  static std::string GetType() { return "SendDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("InterProcAddress address", address) << std::endl;
    ss << format_member("size_t send_position", send_position);
    return ss.str();
  }

};

}
