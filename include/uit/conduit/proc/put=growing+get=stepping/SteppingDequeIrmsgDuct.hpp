#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=growing/DequeIrsendDuct.hpp"
#include "../outlet/get=stepping/SteppingIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct SteppingDequeIrmsgDuct {

  using InletImpl = uit::DequeIrsendDuct<ImplSpec>;
  using OutletImpl = uit::SteppingIrecvDuct<ImplSpec>;

};

} // namespace uit
