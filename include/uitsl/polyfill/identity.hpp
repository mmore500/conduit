#pragma once
#ifndef UITSL_POLYFILL_IDENTITY_HPP_INCLUDE
#define UITSL_POLYFILL_IDENTITY_HPP_INCLUDE

#if __cplusplus < 202002L

#include "impl/identity.hpp"

#else // #if __cplusplus < 202002L

#include <functional>

#endif // #if __cplusplus < 202002L

#endif // #ifndef UITSL_POLYFILL_IDENTITY_HPP_INCLUDE
