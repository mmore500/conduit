#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=dropping/RingIrsendDuct.hpp"
#include "../outlet/get=stepping/SteppingIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct SteppingRingIrmsgDuct {

  using InletImpl = uit::RingIrsendDuct<ImplSpec>;
  using OutletImpl = uit::SteppingIrecvDuct<ImplSpec>;

};

} // namespace uit
