#pragma once
#ifndef UITSL_POLYFILL_BIT_CAST_HPP_INCLUDE
#define UITSL_POLYFILL_BIT_CAST_HPP_INCLUDE

#if __cplusplus < 202002L

#include "impl/bit_cast.hpp"

#else // #if __cplusplus < 202002L

#include <bit>

#endif // #if __cplusplus < 202002L

#endif // #ifndef UITSL_POLYFILL_BIT_CAST_HPP_INCLUDE
