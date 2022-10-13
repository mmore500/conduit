#pragma once
#ifndef UITSL_DEBUG_ERR_VERIFY_HPP_INCLUDE
#define UITSL_DEBUG_ERR_VERIFY_HPP_INCLUDE

#include "../../uit_emp/base/always_assert.hpp"

namespace uitsl {

inline void err_verify(const int err) {
  #include "../../uit_emp/vendorization/push_assert_macros.hh"
  emp_always_assert(err == 0, err);
  #include "../../uit_emp/vendorization/pop_assert_macros.hh"
}

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_ERR_VERIFY_HPP_INCLUDE
