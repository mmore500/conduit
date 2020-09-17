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

#include "../../../../config/InterProcAddress.hpp"

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
  bool TryFlush() { return aggregator->get().TryFlush( address.GetTag() ); }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    throw "ConsumeGets called on AggregatedInletDuct";
  }

  [[noreturn]] const T& Get() const {
    throw "Get called on AggregatedInletDuct";
  }

  [[noreturn]] T& Get() {
    throw "Get called on AggregatedInletDuct";
  }

  static std::string GetType() { return "AggregatedInletDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << std::endl;
    return ss.str();
  }

};

} // namespace uit
