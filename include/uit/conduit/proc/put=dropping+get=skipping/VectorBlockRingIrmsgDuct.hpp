#pragma once

#include "../inlet/put=dropping/VectorRingIrsendDuct.hpp"
#include "../outlet/get=skipping/VectorBlockIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct VectorBlockRingIrmsgDuct {

  using InletImpl = uit::VectorRingIrsendDuct<ImplSpec>;
  using OutletImpl = uit::VectorBlockIrecvDuct<ImplSpec>;

};

} // namespace uit
