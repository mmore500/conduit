#pragma once

#include "../utility/IsFirstExecutionChecker.hpp"

#include "ThreadMap.hpp"

namespace uit {

class OncePerThreadChecker {

  uit::ThreadMap<uit::IsFirstExecutionChecker> map;

public:

  void Check() { map.GetWithDefault().Check(); }

};

}
