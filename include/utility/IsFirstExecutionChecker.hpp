#pragma once

#include <atomic>
#include <stddef.h>

#include "../../third-party/Empirical/source/base/assert.h"

namespace uit {

class IsFirstExecutionChecker {

  std::atomic<size_t> flag{false};

public:

  IsFirstExecutionChecker() = default;

  IsFirstExecutionChecker(const uit::IsFirstExecutionChecker& other)
  : flag(other.flag.load())
  { ; }

  void Check() { emp_assert(0 == flag++); }

};

}
