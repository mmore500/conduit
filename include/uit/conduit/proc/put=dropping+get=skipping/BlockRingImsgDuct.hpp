#pragma once

#include "../inlet/put=dropping/RingIsendDuct.hpp"
#include "../outlet/get=skipping/BlockIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct BlockRingImsgDuct {

  using InletImpl = uit::RingIsendDuct<ImplSpec>;
  using OutletImpl = uit::BlockIrecvDuct<ImplSpec>;

};

} // namespace uit
