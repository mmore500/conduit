#pragma once

#include "ISendDuct.h"
#include "IRecvDuct.h"

namespace uit {

template<typename ImplSpec>
struct IMsgDuct {

  using InletImpl = uit::ISendDuct<ImplSpec>;
  using OutletImpl = uit::IRecvDuct<ImplSpec>;

};

}
