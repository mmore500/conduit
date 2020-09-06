#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=dropping/VectorRingIsendDuct.hpp"
#include "../outlet/get=stepping/VectorIprobeDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct VectorRingImsgDuct {

  using InletImpl = uit::VectorRingIsendDuct<ImplSpec>;
  using OutletImpl = uit::VectorIprobeDuct<ImplSpec>;

};

} // namespace uit
