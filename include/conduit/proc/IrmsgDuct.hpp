#pragma once

#include "IrsendDuct.hpp"
#include "IRecvDuct.hpp"

namespace uit {

template<typename ImplSpec>
struct IrmsgDuct {

  using InletImpl = uit::IrsendDuct<ImplSpec>;
  using OutletImpl = uit::IRecvDuct<ImplSpec>;

};

}
