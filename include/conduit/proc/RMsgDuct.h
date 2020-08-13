#pragma once

#include "RSendDuct.h"
#include "IRecvDuct.h"

namespace uit {

template<typename ImplSpec>
struct RMsgDuct {

  using InletImpl = uit::RSendDuct<ImplSpec>;
  using OutletImpl = uit::IRecvDuct<ImplSpec>;

};

}
