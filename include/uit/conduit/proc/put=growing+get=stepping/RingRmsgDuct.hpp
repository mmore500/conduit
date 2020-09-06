#pragma once

#include "../inlet/put=growing/RsendDuct.hpp"
#include "../outlet/get=stepping/RingIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct RingRmsgDuct {

  using InletImpl = uit::RsendDuct<ImplSpec>;
  using OutletImpl = uit::RingIrecvDuct<ImplSpec>;

};

} // namespace uit
