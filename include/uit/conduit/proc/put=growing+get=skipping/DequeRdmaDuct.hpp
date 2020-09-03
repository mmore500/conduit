#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=growing/DequeRputDuct.hpp"
#include "../outlet/get=skipping/WindowDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct DequeRdmaDuct {

  using InletImpl = uit::DequeRputDuct<ImplSpec>;
  using OutletImpl = uit::WindowDuct<ImplSpec>;

};

} // namespace uit
