#pragma once

#include "SendDuct.hpp"
#include "IRecvDuct.hpp"

namespace uit {

template<typename ImplSpec>
struct MsgDuct {

  using InletImpl = uit::SendDuct<ImplSpec>;
  using OutletImpl = uit::IRecvDuct<ImplSpec>;

};

}
