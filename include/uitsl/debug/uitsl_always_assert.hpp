#pragma once
#ifndef UITSL_DEBUG_UITSL_ALWAYS_ASSERT_HPP_INCLUDE
#define UITSL_DEBUG_UITSL_ALWAYS_ASSERT_HPP_INCLUDE

#include <iostream>
#include <cstdlib>

#include "../utility/UITSL_STRINGIFY.hpp"
#include "../utility/SetSeparator.hpp"

// adapted from https://stackoverflow.com/a/53986818
// pass optional second argument as myvar << "mystring" << etc.
#define uitsl_always_assert(expr, ...)                                        \
do {                                                                          \
  if(!(expr)) {                                                               \
    std::cout << '\n';                                                        \
    std::cout << '\n';                                                        \
    std::cout << "ASSERTION FAILED: ";                                        \
    std::cout << '\n';                                                        \
    std::cout << UITSL_STRINGIFY(expr) << " was " << (expr) << '\n';          \
    std::cout << '\n';                                                        \
    std::cout << "FILE: " << __FILE__ << '\n';                                \
    std::cout << "FUNCTION: " << __PRETTY_FUNCTION__ << '\n';                 \
    std::cout << "LINE: " << __LINE__ << '\n';                                \
    std::cout << '\n';                                                        \
    std::cout << "MESSAGE:\n";                                                \
    std::cout << UITSL_STRINGIFY(__VA_ARGS__) << '\n';                        \
    std::cout << uitsl::SetSeparator(" << ") __VA_OPT__(<<) __VA_ARGS__;      \
    std::cout << '\n';                                                        \
    std::abort();                                                             \
  }                                                                           \
} while(0)

#endif // #ifndef UITSL_DEBUG_UITSL_ALWAYS_ASSERT_HPP_INCLUDE
