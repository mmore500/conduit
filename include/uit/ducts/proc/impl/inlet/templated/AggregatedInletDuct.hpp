#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_TEMPLATED_AGGREGATEDINLETDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_TEMPLATED_AGGREGATEDINLETDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <memory>
#include <stddef.h>
#include <string>

#include <mpi.h>

#include "../../../../../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../../../uitsl/mpi/mpi_utils.hpp"
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
template<
  template<typename> typename BackingDuct,
  typename ImplSpec
>
class AggregatedInletDuct {

public:

  using BackEndImpl = uit::AggregatedBackEnd<ImplSpec, BackingDuct>;

private:

  using T = typename ImplSpec::T;

  uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  using aggregator_t = typename BackEndImpl::inlet_aggregator_t;
  emp::optional<std::reference_wrapper<aggregator_t>> aggregator;

  void SetupAggregator() {
    aggregator = back_end->GetInletAggregator(address);
  }

public:

  AggregatedInletDuct(
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
    if (!aggregator.has_value()) SetupAggregator();
    return aggregator->get().TryPut(val, address.GetTag());
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() {
    if (!aggregator.has_value()) SetupAggregator();
    return aggregator->get().TryFlush( address.GetTag() );
  }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    emp_always_assert(false, "ConsumeGets called on AggregatedInletDuct");
    __builtin_unreachable();
  }

  [[noreturn]] const T& Get() const {
    emp_always_assert(false, "Get called on AggregatedInletDuct");
    __builtin_unreachable();
  }

  [[noreturn]] T& Get() {
    emp_always_assert(false, "Get called on AggregatedInletDuct");
    __builtin_unreachable();
  }

  static std::string GetType() { return "AggregatedInletDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    return ss.str();
  }

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_TEMPLATED_AGGREGATEDINLETDUCT_HPP_INCLUDE
