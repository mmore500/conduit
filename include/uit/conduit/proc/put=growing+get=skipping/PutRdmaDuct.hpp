#pragma once

#include "../backend/MockBackEnd.hpp"
#include "../inlet/put=growing/PutDuct.hpp"
#include "../outlet/get=skipping/WindowDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct PutRdmaDuct {

  using InletImpl = uit::PutDuct<ImplSpec>;
  using OutletImpl = uit::WindowDuct<ImplSpec>;

};

} // namespace uit
