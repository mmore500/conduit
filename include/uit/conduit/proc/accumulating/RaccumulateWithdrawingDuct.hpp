#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/accumulating/RaccumulateDuct.hpp"
#include "../outlet/accumulating/WithdrawingWindowDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct RaccumulateWithdrawingDuct {

  using InletImpl = uit::RaccumulateDuct<ImplSpec>;
  using OutletImpl = uit::WithdrawingWindowDuct<ImplSpec>;

};

} // namespace uit
