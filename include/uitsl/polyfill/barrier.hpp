#pragma once
#ifndef UITSL_POLYFILL_BARRIER_HPP_INCLUDE
#define UITSL_POLYFILL_BARRIER_HPP_INCLUDE

#if __cplusplus < 202002L

#include "impl/barrier.hpp"

#else // #if __cplusplus < 202002L

#include <barrier>

#endif // #if __cplusplus < 202002L

#endif // #ifndef UITSL_POLYFILL_BARRIER_HPP_INCLUDE
