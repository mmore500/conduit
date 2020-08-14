#pragma once

#include "IrecvDuct.hpp"
#include "IsendDuct.hpp"

namespace uit {

template<typename ImplSpec>
struct ImsgDuct {

  using InletImpl = uit::IsendDuct<ImplSpec>;
  using OutletImpl = uit::IrecvDuct<ImplSpec>;

};

}
