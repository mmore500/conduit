#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/accumulating/AccumulateDuct.hpp"
#include "../outlet/accumulating/WithdrawingWindowDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct AccumulateWithdrawingDuct {

  using InletImpl = uit::AccumulateDuct<ImplSpec>;
  using OutletImpl = uit::WithdrawingWindowDuct<ImplSpec>;

};

} // namespace uit
