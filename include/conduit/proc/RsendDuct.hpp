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
#include "../../utility/WarnOnce.hpp"

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
class RsendDuct {

  friend Duct<ImplSpec>;

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using buffer_t = emp::array<T, N>;
  using index_t = CircularIndex<N>;

  buffer_t buffer;

  const uit::InterProcAddress address;

  index_t send_position{0};

  void DoSend() {
    verify(MPI_Rsend(
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

  RsendDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<uit::SharedBackEnd<ImplSpec>> back_end
  ) : address(address_) {
    static const uit::WarnOnce warning{
      "RsendDuct is experimental and may be unreliable"
    };
  }

  ~RsendDuct() {
  }

  //todo rename
  void Push() { DoSend(); }

  void Initialize(const size_t write_position) {
    send_position = write_position;
  }

  //todo rename
  [[noreturn]] void Pop(const size_t count) { throw "bad Pop on RsendDuct"; }

  [[noreturn]] size_t GetPending() { throw "bad GetPending on RsendDuct"; }

  size_t GetAvailableCapacity() { return N; }

  T GetElement(const size_t n) const { return buffer[n]; }

  const void * GetPosition(const size_t n) const { return &buffer[n]; }

  void SetElement(const size_t n, const T & val) { buffer[n] = val; }

  static std::string GetType() { return "RsendDuct"; }

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

} // namespace uit
