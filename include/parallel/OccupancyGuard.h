#pragma once

#include "OccupancyCap.h"

class OccupancyGuard {

  OccupancyCap & space;

public:
  OccupancyGuard(OccupancyCap & space_)
  : space(space_)
  { space.Enter(); }

  ~OccupancyGuard() { space.Exit(); }

};
