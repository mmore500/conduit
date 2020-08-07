#pragma once

#include "ISendDuct.h"
#include "IRecvDuct.h"

namespace uit {

template<typename ImplSpec>
struct IMsgDuct {

  using InletImpl = ISendDuct<ImplSpec>;
  using OutletImpl = IRecvDuct<ImplSpec>;

};

}
