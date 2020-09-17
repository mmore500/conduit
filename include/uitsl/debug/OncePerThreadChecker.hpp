#pragma once

#include "../debug/IsFirstExecutionChecker.hpp"
#include "../parallel/ThreadMap.hpp"

namespace uit {

class OncePerThreadChecker {

  uit::ThreadMap<uit::IsFirstExecutionChecker> map;

public:

  void Check() { map.GetWithDefault().Check(); }

};

} // namespace uit
