#pragma once

#include "IrsendDuct.hpp"
#include "IrecvDuct.hpp"

namespace uit {

template<typename ImplSpec>
struct IrmsgDuct {

  using InletImpl = uit::IrsendDuct<ImplSpec>;
  using OutletImpl = uit::IrecvDuct<ImplSpec>;

};

}
