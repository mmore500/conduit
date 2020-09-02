#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=growing/CerealRsendDuct.hpp"
#include "../outlet/get=skipping/CerealBlockIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct SteppingRmsgDuct {

  using InletImpl = uit::CerealRsendDuct<ImplSpec>;
  using OutletImpl = uit::CerealBlockIrecvDuct<ImplSpec>;

};

} // namespace uit
