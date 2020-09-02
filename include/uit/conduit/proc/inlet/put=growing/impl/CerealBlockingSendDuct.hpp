#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../../third-party/Empirical/source/tools/ContiguousStream.h"
#include "../../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../../distributed/mpi_utils.hpp"
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
class CerealBlockingSendDuct {

public:

  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using buffer_t = emp::array<emp::ContiguousStream, N>;
  buffer_t buffer{};

  using index_t = uit::CircularIndex<N>;
  index_t send_position{};

  const uit::InterProcAddress address;

  void PostSend() {

    BlockingSendFunctor{}(
      buffer[send_position].GetData(),
      buffer[send_position].GetSize(),
      MPI_BYTE,
      address.GetOutletProc(),
      address.GetTag(),
      address.GetComm()
    );
    ++send_position;

  }

public:

  CerealBlockingSendDuct(
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
    { // oarchive flushes on destruction
      buffer[send_position].Reset();
      cereal::BinaryOutputArchive oarchive(
        buffer[send_position]
      );
      oarchive(val);
    }
    PostSend();
    return true;
  }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    throw "ConsumeGets called on CerealBlockingSendDuct";
  }

  [[noreturn]] const T& Get() const {
    throw "Get called on CerealBlockingSendDuct";
  }

  [[noreturn]] T& Get() {
    throw "Get called on CerealBlockingSendDuct";
  }

  static std::string GetType() { return "CerealBlockingSendDuct"; }

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
