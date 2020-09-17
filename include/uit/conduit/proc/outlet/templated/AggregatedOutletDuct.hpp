#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../third-party/Empirical/source/base/optional.h"
#include "../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../uitsl/utility/print_utils.hpp"

#include "../../../InterProcAddress.hpp"

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
  emp::optional<std::reference_wrapper<aggregator_t>> aggregator;

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
    throw "TryPut called on AggregatedOutletDuct";
  }

  [[noreturn]] bool TryFlush() const {
    throw "Flush called on AggregatedOutletDuct";
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
    if (!aggregator.has_value()) SetupAggregator();
    return aggregator->get().Get(address.GetTag());
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
    ss << GetName() << std::endl;
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << std::endl;
    return ss.str();
  }

};

} // namespace uit
