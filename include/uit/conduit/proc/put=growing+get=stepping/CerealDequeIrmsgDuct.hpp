#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=growing/CerealDequeIrsendDuct.hpp"
#include "../outlet/get=stepping/CerealIprobeDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct CerealDequeIrmsgDuct {

  using InletImpl = uit::CerealDequeIrsendDuct<ImplSpec>;
  using OutletImpl = uit::CerealIprobeDuct<ImplSpec>;

};

} // namespace uit
