#pragma once

#include "IrsendDuct.h"
#include "IRecvDuct.h"

namespace uit {

template<typename ImplSpec>
struct IrmsgDuct {

  using InletImpl = uit::IrsendDuct<ImplSpec>;
  using OutletImpl = uit::IRecvDuct<ImplSpec>;

};

}
