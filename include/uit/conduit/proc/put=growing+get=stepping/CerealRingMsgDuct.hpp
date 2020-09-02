#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=growing/SendDuct.hpp"
#include "../outlet/get=stepping/CerealRingIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct CerealRingMsgDuct {

  using InletImpl = uit::SendDuct<ImplSpec>;
  using OutletImpl = uit::CerealRingIrecvDuct<ImplSpec>;

};

} // namespace uit
