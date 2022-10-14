#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_OUTLET_TEMPLATED_POOLEDOUTLETDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_OUTLET_TEMPLATED_POOLEDOUTLETDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <memory>
#include <optional>
#include <stddef.h>
#include <utility>

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
template<template<typename> typename BackingDuct, typename ImplSpec>
class PooledOutletDuct {

public:

  using BackEndImpl = uit::PooledBackEnd<ImplSpec, BackingDuct>;

private:

  using T = typename ImplSpec::T;

  uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  using pool_t = typename BackEndImpl::outlet_pool_t;
  std::optional<std::reference_wrapper<pool_t>> pool;
  size_t pool_index;

  void SetupPool() {
    pool = back_end->GetOutletPool(address);
    pool_index = pool->get().Lookup(address);
  }

public:

  PooledOutletDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_
  ) : address(address_)
  , back_end(back_end_)
  { back_end->RegisterOutletSlot(address); }

  [[noreturn]] bool TryPut(const T&) const {
    uitsl_always_assert(false, "TryPut called on PooledOutletDuct");
    __builtin_unreachable();
  }

  [[noreturn]] bool TryFlush() const {
    uitsl_always_assert(false, "Flush called on PooledOutletDuct");
    __builtin_unreachable();
  }

  /**
   * TODO.
   *
   * @param num_requested TODO.
   * @return number items consumed.
   */
  size_t TryConsumeGets(const size_t num_requested) {

    if (!pool.has_value()) SetupPool();
    return pool->get().TryConsumeGets(num_requested, address);

  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const {
    if (!pool.has_value()) const_cast<PooledOutletDuct*>(this)->SetupPool();
    return std::as_const(pool->get()).Get(pool_index);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() {
    if (!pool.has_value()) SetupPool();
    return pool->get().Get(pool_index);
  }

  static std::string GetName() { return "PooledOutletDuct"; }

  static constexpr bool CanStep() {
    return BackingDuct<ImplSpec>::OutletImpl::CanStep();
  }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    return ss.str();
  }

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_OUTLET_TEMPLATED_POOLEDOUTLETDUCT_HPP_INCLUDE
