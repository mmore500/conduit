#pragma once
#ifndef UITSL_COUNTDOWN_COARSE_RUNTIME_HPP_INCLUDE
#define UITSL_COUNTDOWN_COARSE_RUNTIME_HPP_INCLUDE

#include <chrono>

#include "../chrono/CoarseClock.hpp"

#include "Timer.hpp"

#include "runtime.hpp"

namespace uitsl {


template<
  typename Duration_T=std::chrono::duration<double, std::ratio<1>>
>
uitsl::Timer<Duration_T, uitsl::CoarseClock> coarse_runtime;


} // namespace uitsl

#endif // #ifndef UITSL_COUNTDOWN_COARSE_RUNTIME_HPP_INCLUDE
