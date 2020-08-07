#pragma once

#include "R.h"
#include "IRecvDuct.h"

namespace uit {

template<typename ImplSpec>
struct RdmaDuct {

  using InletImpl = RputDuct<ImplSpec>;
  using OutletImpl = WindowDuct<ImplSpec>;

};

}
