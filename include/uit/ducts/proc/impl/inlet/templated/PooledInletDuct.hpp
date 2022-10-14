#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_TEMPLATED_POOLEDINLETDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_TEMPLATED_POOLEDINLETDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <memory>
#include <optional>
#include <stddef.h>
#include <string>

#include <mpi.h>

#include "../../../../../../uitsl/debug/uitsl_always_assert.hpp"
#include "../../../../../../uitsl/mpi/mpi_init_utils.hpp"
#include "../../../../../../uitsl/utility/print_utils.hpp"

#include "../../../../../setup/InterProcAddress.hpp"

#include "../../backend/PooledBackEnd.hpp"

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
class PooledInletDuct {

public:

  using BackEndImpl = uit::PooledBackEnd<ImplSpec, BackingDuct>;

private:

  using T = typename ImplSpec::T;

  uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  using pool_t = typename BackEndImpl::inlet_pool_t;
  std::optional<std::reference_wrapper<pool_t>> pool;
  size_t pool_index;

  void SetupPool() {
    pool = back_end->GetInletPool(address);
    pool_index = pool->get().Lookup(address);
  }

public:

  PooledInletDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_
  ) : address(address_)
  , back_end(back_end_)
  { back_end->RegisterInletSlot(address); }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    if (!pool.has_value()) SetupPool();
    return pool->get().TryPut(val, pool_index);
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() {
    if (!pool.has_value()) SetupPool();
    return pool->get().TryFlush();
  }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    uitsl_always_assert(false, "ConsumeGets called on PooledInletDuct");
    __builtin_unreachable();
  }

  [[noreturn]] const T& Get() const {
    uitsl_always_assert(false, "Get called on PooledInletDuct");
    __builtin_unreachable();
  }

  [[noreturn]] T& Get() {
    uitsl_always_assert(false, "Get called on PooledInletDuct");
    __builtin_unreachable();
  }

  static std::string GetType() { return "PooledInletDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    return ss.str();
  }

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_TEMPLATED_POOLEDINLETDUCT_HPP_INCLUDE
