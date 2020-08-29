#pragma once

#include "backend/MockBackEnd.hpp"
#include "inlet/IrsendDuct.hpp"
#include "outlet/IrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IrmsgDuct {

  using InletImpl = uit::IrsendDuct<ImplSpec>;
  using OutletImpl = uit::IrecvDuct<ImplSpec>;

};

} // namespace uit
