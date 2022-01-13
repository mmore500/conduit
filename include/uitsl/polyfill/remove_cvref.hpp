#pragma once
#ifndef UITSL_POLYFILL_REMOVE_CVREF_HPP_INCLUDE
#define UITSL_POLYFILL_REMOVE_CVREF_HPP_INCLUDE

#if __cplusplus < 202002L

#include "impl/remove_cvref.hpp"

#else // #if __cplusplus < 202002L

#include <type_traits>

#endif // #if __cplusplus < 202002L

#endif // #ifndef UITSL_POLYFILL_REMOVE_CVREF_HPP_INCLUDE
