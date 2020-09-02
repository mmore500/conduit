#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=growing/CerealDequeIrsendDuct.hpp"
#include "../outlet/get=skipping/CerealBlockIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct CerealBlockDequeIrmsgDuct {

  using InletImpl = uit::CerealDequeIrsendDuct<ImplSpec>;
  using OutletImpl = uit::CerealBlockIrecvDuct<ImplSpec>;

};

} // namespace uit
