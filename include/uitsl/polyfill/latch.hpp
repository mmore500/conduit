#pragma once
#ifndef UITSL_POLYFILL_LATCH_HPP_INCLUDE
#define UITSL_POLYFILL_LATCH_HPP_INCLUDE

#if __cplusplus < 202002L

#include "impl/latch.hpp"

#else // #if __cplusplus < 202002L

#include <latch>

#endif // #if __cplusplus < 202002L

#endif // #ifndef UITSL_POLYFILL_LATCH_HPP_INCLUDE
