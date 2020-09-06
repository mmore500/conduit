#pragma once

#include "../inlet/put=dropping/VectorRingIsendDuct.hpp"
#include "../outlet/get=skipping/VectorBlockIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct VectorBlockRingImsgDuct {

  using InletImpl = uit::VectorRingIsendDuct<ImplSpec>;
  using OutletImpl = uit::VectorBlockIrecvDuct<ImplSpec>;

};

} // namespace uit
