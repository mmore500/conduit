#pragma once

#include "../inlet/put=dropping/CerealRingIrsendDuct.hpp"
#include "../outlet/get=stepping/CerealIprobeDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct CerealRingIrmsgDuct {

  using InletImpl = uit::CerealRingIrsendDuct<ImplSpec>;
  using OutletImpl = uit::CerealIprobeDuct<ImplSpec>;

};

} // namespace uit
