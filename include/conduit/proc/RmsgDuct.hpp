#pragma once

#include "IrecvDuct.hpp"
#include "RsendDuct.hpp"

namespace uit {

template<typename ImplSpec>
struct RmsgDuct {

  using InletImpl = uit::RsendDuct<ImplSpec>;
  using OutletImpl = uit::IrecvDuct<ImplSpec>;

};

}
