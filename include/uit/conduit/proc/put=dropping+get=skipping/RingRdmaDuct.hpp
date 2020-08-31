#pragma once

#include "../backend/RdmaBackEnd.hpp"
#include "../inlet/put=dropping/RingRputDuct.hpp"
#include "../outlet/get=skipping/WindowDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct RingRdmaDuct {

  using InletImpl = uit::RingRputDuct<ImplSpec>;
  using OutletImpl = uit::WindowDuct<ImplSpec>;

};

} // namespace uit
