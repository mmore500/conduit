#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include <stddef.h>

#include <mpi.h>

#include "../../../third-party/Empirical/source/base/assert.h"
#include "../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../distributed/mpi_utils.hpp"
#include "../../utility/CircularIndex.hpp"
#include "../../utility/identity.hpp"
#include "../../utility/print_utils.hpp"

#include "../InterProcAddress.hpp"
#include "../proc/backend/MockBackEnd.hpp"

namespace uit {
namespace internal {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename BlockingSendFunctor, typename ImplSpec>
class BlockingSendDuct {

public:

  using BackEndImpl = uit::internal::MockBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using buffer_t = emp::array<T, N>;
  buffer_t buffer{};

  using index_t = uit::CircularIndex<N>;
  index_t send_position{};

  const uit::InterProcAddress address;

  void PostSend() {

    BlockingSendFunctor{}(
      &buffer[send_position],
      sizeof(T),
      MPI_BYTE,
      address.GetOutletProc(),
      address.GetTag(),
      address.GetComm()
    );
    ++send_position;

  }

public:

  BlockingSendDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end
  ) : address(address_)
  { ; }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  void Put(const T& val) {
    buffer[send_position] = val;
    PostSend();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  bool IsReadyForPut() const { return true; }

  [[noreturn]] size_t TryConsumeGets(const size_t requested) {
    throw "ConsumeGets called on BlockingSendDuct";
  }

  [[noreturn]] const T& Get() const { throw "Get called on BlockingSendDuct"; }

  static std::string GetType() { return "BlockingSendDuct"; }

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

} // namespace internal
} // namespace uit
