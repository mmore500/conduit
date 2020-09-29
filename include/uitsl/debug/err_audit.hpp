#pragma once
#ifndef UITSL_DEBUG_ERR_AUDIT_HPP_INCLUDE
#define UITSL_DEBUG_ERR_AUDIT_HPP_INCLUDE

#include "../../../third-party/Empirical/source/base/assert.h"

namespace uitsl {

inline void err_audit(const int err) { emp_assert(err == 0, err); }

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_ERR_AUDIT_HPP_INCLUDE
