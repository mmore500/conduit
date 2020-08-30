#pragma once

#include "backend/RdmaBackEnd.hpp"
#include "inlet/RputDuct.hpp"
#include "outlet/WindowDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct RdmaDuct {

  using InletImpl = uit::RputDuct<ImplSpec>;
  using OutletImpl = uit::WindowDuct<ImplSpec>;

};

} // namespace uit
