#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=growing/DequeIrsendDuct.hpp"
#include "../outlet/get=stepping/RingIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct RingDequeIrmsgDuct {

  using InletImpl = uit::DequeIrsendDuct<ImplSpec>;
  using OutletImpl = uit::RingIrecvDuct<ImplSpec>;

};

} // namespace uit
