#pragma once

#include "../debug/IsFirstExecutionChecker.hpp"
#include "../parallel/ThreadMap.hpp"

namespace uitsl {

class OncePerThreadChecker {

  uitsl::ThreadMap<uitsl::IsFirstExecutionChecker> map;

public:

  void Check() { map.GetWithDefault().Check(); }

};

} // namespace uitsl
