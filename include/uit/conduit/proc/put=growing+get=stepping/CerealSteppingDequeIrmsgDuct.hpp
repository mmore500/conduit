#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=growing/CerealDequeIrsendDuct.hpp"
#include "../outlet/get=stepping/CerealSteppingIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct CerealSteppingDequeIrmsgDuct {

  using InletImpl = uit::CerealDequeIrsendDuct<ImplSpec>;
  using OutletImpl = uit::CerealSteppingIrecvDuct<ImplSpec>;

};

} // namespace uit
