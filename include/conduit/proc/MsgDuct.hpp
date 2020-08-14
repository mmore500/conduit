#pragma once

#include "IrecvDuct.hpp"
#include "SendDuct.hpp"

namespace uit {

template<typename ImplSpec>
struct MsgDuct {

  using InletImpl = uit::SendDuct<ImplSpec>;
  using OutletImpl = uit::IrecvDuct<ImplSpec>;

};

}
