#pragma once

#include "../../../third-party/Empirical/source/base/errors.h"

namespace uitsl {

class WarnOnce {

public:

  WarnOnce(const std::string& warning) {
    emp::NotifyWarning(warning);
  }

};

} // namespace uitsl
