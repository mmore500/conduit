#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../../mpi/mpi_utils.hpp"
#include "../../../../../utility/CircularIndex.hpp"
#include "../../../../../utility/identity.hpp"
#include "../../../../../utility/print_utils.hpp"

#include "../../../../InterProcAddress.hpp"

#include "../../../backend/MockBackEnd.hpp"

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

  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  T buffer{};

  const uit::InterProcAddress address;

  void PostSend() {

    BlockingSendFunctor{}(
      &buffer,
      sizeof(T),
      MPI_BYTE,
      address.GetOutletProc(),
      address.GetTag(),
      address.GetComm()
    );

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
  bool TryPut(const T& val) {
    buffer = val;
    PostSend();
    return true;
  }

  /**
   * TODO.
   *
   */
  void Flush() const { ; }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    throw "ConsumeGets called on BlockingSendDuct";
  }

  [[noreturn]] const T& Get() const { throw "Get called on BlockingSendDuct"; }

  [[noreturn]] T& Get() { throw "Get called on BlockingSendDuct"; }

  static std::string GetType() { return "BlockingSendDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("T buffer", buffer) << std::endl;
    ss << format_member("InterProcAddress address", address) << std::endl;
    return ss.str();
  }

};

} // namespace internal
} // namespace uit
