#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_OUTLET_TEMPLATED_AGGREGATEDOUTLETDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_OUTLET_TEMPLATED_AGGREGATEDOUTLETDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <memory>
#include <optional>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../uitsl/debug/uitsl_always_assert.hpp"
#include "../../../../../../uitsl/mpi/mpi_init_utils.hpp"
#include "../../../../../../uitsl/utility/print_utils.hpp"

#include "../../../../../setup/InterProcAddress.hpp"

#include "../../backend/AggregatedBackEnd.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<template<typename> typename BackingDuct, typename ImplSpec>
class AggregatedOutletDuct {

public:

  using BackEndImpl = uit::AggregatedBackEnd<ImplSpec, BackingDuct>;

private:

  using T = typename ImplSpec::T;

  uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  using aggregator_t = typename BackEndImpl::outlet_aggregator_t;
  std::optional<std::reference_wrapper<aggregator_t>> aggregator;

  void SetupAggregator() {
    aggregator = back_end->GetOutletAggregator(address);
  }

public:

  AggregatedOutletDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_
  ) : address(address_)
  , back_end(back_end_)
  { back_end->RegisterOutletSlot(address); }

  [[noreturn]] bool TryPut(const T&) const {
    uitsl_always_assert(false, "TryPut called on AggregatedOutletDuct");
    __builtin_unreachable();
  }

  [[noreturn]] bool TryFlush() const {
    uitsl_always_assert(false, "Flush called on AggregatedOutletDuct");
    __builtin_unreachable();
  }

  /**
   * TODO.
   *
   * @param num_requested TODO.
   * @return number items consumed.
   */
  size_t TryConsumeGets(const size_t num_requested) {

    if (!aggregator.has_value()) SetupAggregator();
    return aggregator->get().TryConsumeGets(num_requested, address.GetTag());

  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const {
    if (!aggregator.has_value()) {
      const_cast<AggregatedOutletDuct*>(this)->SetupAggregator();
    }
    return std::as_const(aggregator->get()).Get(address.GetTag());
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() {
    if (!aggregator.has_value()) SetupAggregator();
    return aggregator->get().Get(address.GetTag());
  }

  static std::string GetName() { return "AggregatedOutletDuct"; }

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

#endif // #ifndef UIT_DUCTS_PROC_IMPL_OUTLET_TEMPLATED_AGGREGATEDOUTLETDUCT_HPP_INCLUDE
