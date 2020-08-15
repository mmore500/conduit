#pragma once

#include "RputDuct.hpp"
#include "WindowDuct.hpp"

namespace uit {

template<typename ImplSpec>
struct RdmaDuct {

  using InletImpl = uit::RputDuct<ImplSpec>;
  using OutletImpl = uit::WindowDuct<ImplSpec>;

};

}
