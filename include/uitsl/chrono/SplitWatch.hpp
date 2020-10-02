#pragma once

#include <chrono>

#include "../../../third-party/Empirical/source/base/optional.h"

namespace uitsl {

template<typename Clock>
class SplitWatch {

  using time_point_t = std::chrono::time_point<Clock>;
  using duration_t = typename Clock::duration;

  emp::optional< time_point_t > last;

public:

  duration_t TakeSplit() {

    const time_point_t cur{ Clock::now() };

    const auto res = cur - last.value_or(cur);

    last = cur;

    return res;

  }

};

} // namespace uitsl
