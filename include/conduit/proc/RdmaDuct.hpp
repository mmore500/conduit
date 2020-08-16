#pragma once

#include "RputDuct.hpp"
#include "WindowDuct.hpp"

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

}
