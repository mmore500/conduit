#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=dropping/RingIrsendDuct.hpp"
#include "../outlet/get=skipping/BlockIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct BlockRingIrmsgDuct {

  using InletImpl = uit::RingIrsendDuct<ImplSpec>;
  using OutletImpl = uit::BlockIrecvDuct<ImplSpec>;

};

} // namespace uit
