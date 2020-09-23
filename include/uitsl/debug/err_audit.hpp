#pragma once

#include "../../../third-party/Empirical/source/base/assert.h"

namespace uitsl {

inline void err_audit(const int err) { emp_assert(err == 0, err); }

} // namespace uitsl
