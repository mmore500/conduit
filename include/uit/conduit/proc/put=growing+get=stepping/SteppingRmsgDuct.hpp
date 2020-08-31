#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=growing/RsendDuct.hpp"
#include "../outlet/get=stepping/SteppingIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct SteppingRmsgDuct {

  using InletImpl = uit::RsendDuct<ImplSpec>;
  using OutletImpl = uit::SteppingIrecvDuct<ImplSpec>;

};

} // namespace uit
