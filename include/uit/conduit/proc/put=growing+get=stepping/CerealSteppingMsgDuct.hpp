#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=growing/SendDuct.hpp"
#include "../outlet/get=stepping/CerealSteppingIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct CerealSteppingMsgDuct {

  using InletImpl = uit::SendDuct<ImplSpec>;
  using OutletImpl = uit::CerealSteppingIrecvDuct<ImplSpec>;

};

} // namespace uit
