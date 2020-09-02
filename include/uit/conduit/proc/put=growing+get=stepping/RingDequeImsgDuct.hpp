#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=growing/DequeIsendDuct.hpp"
#include "../outlet/get=stepping/RingIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct RingDequeImsgDuct {

  using InletImpl = uit::DequeIsendDuct<ImplSpec>;
  using OutletImpl = uit::RingIrecvDuct<ImplSpec>;

};

} // namespace uit
