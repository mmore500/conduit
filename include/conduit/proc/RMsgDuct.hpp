#pragma once

#include "RSendDuct.hpp"
#include "IRecvDuct.hpp"

namespace uit {

template<typename ImplSpec>
struct RMsgDuct {

  using InletImpl = uit::RSendDuct<ImplSpec>;
  using OutletImpl = uit::IRecvDuct<ImplSpec>;

};

}
