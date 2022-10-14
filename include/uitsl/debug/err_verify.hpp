#pragma once
#ifndef UITSL_DEBUG_ERR_VERIFY_HPP_INCLUDE
#define UITSL_DEBUG_ERR_VERIFY_HPP_INCLUDE

#include "../../uitsl/debug/uitsl_always_assert.hpp"

namespace uitsl {

inline void err_verify(const int err) {
  uitsl_always_assert(err == 0, err);
}

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_ERR_VERIFY_HPP_INCLUDE
