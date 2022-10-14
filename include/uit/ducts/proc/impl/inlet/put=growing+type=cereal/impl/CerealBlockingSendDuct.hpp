#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_GROWING_TYPE_CEREAL_IMPL_CEREALBLOCKINGSENDDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_PUT_GROWING_TYPE_CEREAL_IMPL_CEREALBLOCKINGSENDDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <memory>
#include <stddef.h>
#include <string>

#include <mpi.h>

#include "../../../../../../../uit_emp/io/ContiguousStream.hpp"

#include "../../../../../../../uitsl/debug/uitsl_always_assert.hpp"
#include "../../../../../../../uitsl/meta/c::static_test.hpp"
#include "../../../../../../../uitsl/mpi/mpi_init_utils.hpp"
#include "../../../../../../../uitsl/nonce/CircularIndex.hpp"
#include "../../../../../../../uitsl/utility/print_utils.hpp"

#include "../../../../../../setup/InterProcAddress.hpp"

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
  static_assert( uitsl::c::static_test<T>(), uitsl_c_message );
  constexpr inline static size_t N{ImplSpec::N};

  uit_emp::ContiguousStream buffer;

  const uit::InterProcAddress address;

  void PostSend() {

    BlockingSendFunctor{}(
      buffer.GetData(),
      buffer.GetSize(),
      MPI_BYTE,
      address.GetOutletProc(),
      address.GetTag(),
      address.GetComm()
    );

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
      buffer.Reset();
      cereal::BinaryOutputArchive oarchive( buffer );
      oarchive(val);
    }
    PostSend();
    return true;
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() const { return true; }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    uitsl_always_assert(false, "ConsumeGets called on CerealBlockingSendDuct");
    __builtin_unreachable();
  }

  [[noreturn]] const T& Get() const {
    uitsl_always_assert(false, "Get called on CerealBlockingSendDuct");
    __builtin_unreachable();
  }

  [[noreturn]] T& Get() {
    uitsl_always_assert(false, "Get called on CerealBlockingSendDuct");
    __builtin_unreachable();
  }

  static std::string GetType() { return "CerealBlockingSendDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    ss << uitsl::format_member("InterProcAddress address", address) << '\n';
    return ss.str();
  }

};

} // namespace internal
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_GROWING_TYPE_CEREAL_IMPL_CEREALBLOCKINGSENDDUCT_HPP_INCLUDE
