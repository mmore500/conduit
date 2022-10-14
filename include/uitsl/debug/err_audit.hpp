#pragma once
#ifndef UITSL_DEBUG_ERR_AUDIT_HPP_INCLUDE
#define UITSL_DEBUG_ERR_AUDIT_HPP_INCLUDE

#include "../../uitsl/debug/uitsl_assert.hpp"

namespace uitsl {

inline void err_audit(const int err) {
  uitsl_assert(err == 0, err);
}

inline void err_audit(
  const int err, const int line, const std::string& file
) { uitsl_assert(err == 0, err << line << file); }

} // namespace uitsl

#define uitsl_err_audit(err) uitsl::err_audit(err, __LINE__, __FILE__)

#endif // #ifndef UITSL_DEBUG_ERR_AUDIT_HPP_INCLUDE
