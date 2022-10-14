#pragma once
#ifndef UITSL_DEBUG_UITSL_ALWAYS_ASSERT_HPP_INCLUDE
#define UITSL_DEBUG_UITSL_ALWAYS_ASSERT_HPP_INCLUDE

#include <iostream>
#include <cstdlib>

#include "../meta/is_streamable.hpp"
#include "../meta/fwd_or_cast.hpp"
#include "../meta/UITSL_NAMESPACE.hpp"
#include "../utility/UITSL_STRINGIFY.hpp"
#include "../utility/SetSeparator.hpp"

// adapted from https://stackoverflow.com/a/53986818
// pass optional second argument as myvar << "mystring" << etc.
#define uitsl_always_assert(expr, ...)                                        \
do {                                                                          \
  if(!(expr)) {                                                               \
    namespace uitsl_ns = UITSL_NAMESPACE();                                   \
    std::cout << '\n';                                                        \
    std::cout << '\n';                                                        \
    std::cout << "ASSERTION FAILED: ";                                        \
    std::cout << '\n';                                                        \
    std::cout << UITSL_STRINGIFY(expr) << " was ";                            \
    std::cout << uitsl_ns::fwd_or_cast<uitsl_ns::is_streamable, bool>(expr);  \
    std::cout << '\n';                                                        \
    std::cout << '\n';                                                        \
    std::cout << "FILE: " << __FILE__ << '\n';                                \
    std::cout << "FUNCTION: " << __PRETTY_FUNCTION__ << '\n';                 \
    std::cout << "LINE: " << __LINE__ << '\n';                                \
    std::cout << '\n';                                                        \
    std::cout << "MESSAGE:\n";                                                \
    std::cout << UITSL_STRINGIFY(__VA_ARGS__) << '\n';                        \
    std::cout << uitsl_ns::SetSeparator(" << ") __VA_OPT__(<<) __VA_ARGS__;   \
    std::cout << '\n';                                                        \
    std::abort();                                                             \
  }                                                                           \
} while(0)

#endif // #ifndef UITSL_DEBUG_UITSL_ALWAYS_ASSERT_HPP_INCLUDE
