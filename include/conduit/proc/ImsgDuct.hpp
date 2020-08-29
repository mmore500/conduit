#pragma once

#include "backend/MockBackEnd.hpp"
#include "inlet/IsendDuct.hpp"
#include "outlet/IrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct ImsgDuct {

  using InletImpl = uit::IsendDuct<ImplSpec>;
  using OutletImpl = uit::IrecvDuct<ImplSpec>;

};

} // namespace uit
