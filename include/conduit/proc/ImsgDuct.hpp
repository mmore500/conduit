#pragma once

#include "IsendDuct.hpp"
#include "IrecvDuct.hpp"

namespace uit {

template<typename ImplSpec>
struct ImsgDuct {

  using InletImpl = uit::IsendDuct<ImplSpec>;
  using OutletImpl = uit::IrecvDuct<ImplSpec>;

};

}
