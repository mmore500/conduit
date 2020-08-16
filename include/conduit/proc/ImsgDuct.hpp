#pragma once

#include "IrecvDuct.hpp"
#include "IsendDuct.hpp"

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

}
