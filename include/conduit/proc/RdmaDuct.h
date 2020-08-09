#pragma once

#include "RputDuct.h"
#include "WindowDuct.h"

namespace uit {

template<typename ImplSpec>
struct RdmaDuct {

  using InletImpl = uit::RputDuct<ImplSpec>;
  using OutletImpl = uit::WindowDuct<ImplSpec>;

};

}
