#pragma once

#include "IrecvDuct.hpp"
#include "IrsendDuct.hpp"

namespace uit {

template<typename ImplSpec>
struct IrmsgDuct {

  using InletImpl = uit::IrsendDuct<ImplSpec>;
  using OutletImpl = uit::IrecvDuct<ImplSpec>;

};

}
