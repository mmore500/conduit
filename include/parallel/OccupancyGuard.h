#pragma once

#include "OccupancyCap.h"

namespace uit {

class OccupancyGuard {

  OccupancyCap & space;

public:
  OccupancyGuard(OccupancyCap & space_)
  : space(space_)
  { space.Enter(); }

  ~OccupancyGuard() { space.Exit(); }

};

}
