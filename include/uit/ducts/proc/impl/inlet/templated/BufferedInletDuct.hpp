#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_TEMPLATED_BUFFEREDINLETDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_TEMPLATED_BUFFEREDINLETDUCT_HPP_INCLUDE

#include <algorithm>
#include <stddef.h>
#include <string>

#include <mpi.h>

#include "../../../../../../uitsl/debug/uitsl_always_assert.hpp"
#include "../../../../../../uitsl/mpi/mpi_init_utils.hpp"
#include "../../../../../../uitsl/utility/print_utils.hpp"

#include "../../../../../setup/InterProcAddress.hpp"

#include "impl/BufferSpec.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<
  template<typename> typename BackingDuct,
  typename ImplSpec
>
class BufferedInletDuct {

  using BufferSpec = uit::internal::BufferSpec<ImplSpec, BackingDuct>;

public:

  using BackEndImpl = typename BackingDuct<BufferSpec>::BackEndImpl;

private:

  using T = typename ImplSpec::T;
  constexpr inline static size_t B{ ImplSpec::B };

  using BackingInlet = typename BackingDuct<BufferSpec>::InletImpl;
  BackingInlet inlet;

  using buffer_t = typename BufferSpec::T;
  buffer_t buffer;

public:

  BufferedInletDuct(
    const uit::InterProcAddress& address,
    std::shared_ptr<BackEndImpl> back_end
  ) : inlet{address, back_end}
  { ; }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    if (buffer.size() < B) {
      buffer.push_back(val);
      return true;
    } else return false;
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  template<typename P>
  bool TryPut(P&& val) {
    if (buffer.size() < B) {
      buffer.push_back( std::forward<P>(val) );
      return true;
    } else return false;
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() {
    if ( buffer.size() ) {
      const bool res = inlet.TryPut( std::move(buffer) );
      buffer.clear();
      return res;
    }
    else return true;
  }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    uitsl_always_assert(false, "ConsumeGets called on BufferedInletDuct");
    __builtin_unreachable();
  }

  [[noreturn]] const T& Get() const {
    uitsl_always_assert(false, "Get called on BufferedInletDuct");
    __builtin_unreachable();
  }

  [[noreturn]] T& Get() {
    uitsl_always_assert(false, "Get called on BufferedInletDuct");
    __builtin_unreachable();
  }

  static std::string GetType() { return "BufferedInletDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    return ss.str();
  }

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_TEMPLATED_BUFFEREDINLETDUCT_HPP_INCLUDE
