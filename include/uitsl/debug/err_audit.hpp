#pragma once
#ifndef UITSL_DEBUG_ERR_AUDIT_HPP_INCLUDE
#define UITSL_DEBUG_ERR_AUDIT_HPP_INCLUDE

#include "../../uit_emp/base/assert.hpp"
#include "../../uit_emp/vendorization/push_assert_macros.hh"

namespace uitsl {

inline void err_audit(const int err) {
  emp_assert(err == 0, err);
}

inline void err_audit(
  const int err, const int line, const std::string& file
) { emp_assert(err == 0, err, line, file); }

} // namespace uitsl

#define uitsl_err_audit(err) uitsl::err_audit(err, __LINE__, __FILE__)

#include "../../uit_emp/vendorization/pop_assert_macros.hh"
#endif // #ifndef UITSL_DEBUG_ERR_AUDIT_HPP_INCLUDE
