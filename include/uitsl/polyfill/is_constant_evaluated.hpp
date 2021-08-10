#pragma once
#ifndef UITSL_POLYFILL_IS_CONSTANT_EVALUATED_HPP_INCLUDE
#define UITSL_POLYFILL_IS_CONSTANT_EVALUATED_HPP_INCLUDE

#if __cplusplus < 202002L

#include "impl/is_constant_evaluated.hpp"

#else // #if __cplusplus < 202002L

#include <type_traits>

#endif // #if __cplusplus < 202002L

#endif // #ifndef UITSL_POLYFILL_IS_CONSTANT_EVALUATED_HPP_INCLUDE
