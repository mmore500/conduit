#pragma once

#include <chrono>
#include <limits>

namespace uit {

std::chrono::duration<double> infinite_duration {
  std::numeric_limits<double>::infinity()
};

}

// std::chrono::duration<int> zero_duration {
//   0
// };
