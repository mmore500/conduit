#pragma once

#include "ISendDuct.hpp"
#include "IRecvDuct.hpp"

namespace uit {

template<typename ImplSpec>
struct IMsgDuct {

  using InletImpl = uit::ISendDuct<ImplSpec>;
  using OutletImpl = uit::IRecvDuct<ImplSpec>;

};

}
