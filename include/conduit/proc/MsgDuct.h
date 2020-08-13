#pragma once

#include "SendDuct.h"
#include "IRecvDuct.h"

namespace uit {

template<typename ImplSpec>
struct MsgDuct {

  using InletImpl = uit::SendDuct<ImplSpec>;
  using OutletImpl = uit::IRecvDuct<ImplSpec>;

};

}
