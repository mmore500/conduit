#pragma once

#include "../inlet/put=dropping/RingIsendDuct.hpp"
#include "../outlet/get=stepping/RingIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct RingRingImsgDuct {

  using InletImpl = uit::RingIsendDuct<ImplSpec>;
  using OutletImpl = uit::RingIrecvDuct<ImplSpec>;

};

} // namespace uit
