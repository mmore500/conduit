#pragma once
#ifndef UITSL_DEBUG_UITSL_ASSERT_HPP_INCLUDE
#define UITSL_DEBUG_UITSL_ASSERT_HPP_INCLUDE

#include "uitsl_always_assert.hpp"

#ifndef NDEBUG
#define uitsl_assert(expr, ...) uitsl_always_assert(expr, __VA_ARGS__)
#else
#define uitsl_assert(expr, ...)
#endif // #ifndef NDEBUG

#endif // #ifndef UITSL_DEBUG_UITSL_ASSERT_HPP_INCLUDE
