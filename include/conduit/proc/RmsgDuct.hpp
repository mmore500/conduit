#pragma once

#include "RsendDuct.hpp"
#include "IrecvDuct.hpp"

namespace uit {

template<typename ImplSpec>
struct RmsgDuct {

  using InletImpl = uit::RsendDuct<ImplSpec>;
  using OutletImpl = uit::IrecvDuct<ImplSpec>;

};

}
