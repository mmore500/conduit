#pragma once
#ifndef UITSL_POLYFILL_ERASE_IF_HPP_INCLUDE
#define UITSL_POLYFILL_ERASE_IF_HPP_INCLUDE

#if __cplusplus < 202002L

#include "impl/erase_if.hpp"

#else // #if __cplusplus < 202002L

#include <vector>

#endif // #if __cplusplus < 202002L

#endif // #ifndef UITSL_POLYFILL_ERASE_IF_HPP_INCLUDE
