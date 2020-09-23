#pragma once

#include <atomic>
#include <stddef.h>

#include "../../../third-party/Empirical/source/base/assert.h"

namespace uitsl {

class IsFirstExecutionChecker {

  std::atomic<size_t> flag{false};

public:

  IsFirstExecutionChecker() = default;

  IsFirstExecutionChecker(const uitsl::IsFirstExecutionChecker& other)
  : flag(other.flag.load())
  { ; }

  void Check() { emp_assert(0 == flag++); }

};

} // namespace uitsl
