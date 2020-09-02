#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=growing/CerealDequeIsendDuct.hpp"
#include "../outlet/get=stepping/CerealRingIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct CerealRingDequeImsgDuct {

  using InletImpl = uit::CerealDequeIsendDuct<ImplSpec>;
  using OutletImpl = uit::CerealRingIrecvDuct<ImplSpec>;

};

} // namespace uit
