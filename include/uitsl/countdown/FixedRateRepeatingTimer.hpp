#pragma once
#ifndef UITSL_COUNTDOWN_FIXEDRATEREPEATINGTIMER_HPP_INCLUDE
#define UITSL_COUNTDOWN_FIXEDRATEREPEATINGTIMER_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <stddef.h>

#include "../chrono/chrono_utils.hpp"
#include "../chrono/CoarseClock.hpp"
#include "../chrono/CoarseRealClock.hpp"

#include "impl/CountdownIterator.hpp"

namespace uitsl {

template<
  typename Duration_T=std::chrono::duration<double, std::ratio<1>>,
  typename Clock_T=std::chrono::steady_clock
>
class FixedRateRepeatingTimer {

  Duration_T duration;
  mutable size_t elapsed_cycles{};

  std::chrono::time_point<Clock_T> start{ Clock_T::now() };

public:

  using iterator = uitsl::CountdownIterator<FixedRateRepeatingTimer>;
  using elapsed_t = Duration_T;

  explicit FixedRateRepeatingTimer(
    const Duration_T& duration_=infinite_duration
  ) : duration(duration_)
  { ; }

  explicit FixedRateRepeatingTimer( const double duration_ )
  : FixedRateRepeatingTimer( Duration_T{ duration_ } )
  { ; }

  Duration_T GetElapsedUnderCompleteCycles() const {
    return duration * elapsed_cycles;
  }

  // a successful IsComplete call advances the timer one cycle
  bool IsComplete() const {
    const bool res = duration <= GetElapsed();
    elapsed_cycles += res;
    return res;
  }

  Duration_T GetElapsed() const {
    return std::chrono::duration_cast<Duration_T>(
      Clock_T::now() - start
    ) - GetElapsedUnderCompleteCycles();
  }

  Duration_T GetRemaining() const {
    return duration - GetElapsed();
  }

  double GetFractionComplete() const {
    return (
      static_cast<double>( GetElapsed().count() )
      / static_cast<double>( duration.count() )
    );
  }

  FixedRateRepeatingTimer& operator++() { return *this; }

  iterator begin() { return iterator{ *this }; }

  iterator end() { return iterator{}; }

  void Reset() { start = Clock_T::now(); elapsed_cycles = 0; }

};

// convenience typedef
using CoarseFixedRateRepeatingTimer = uitsl::FixedRateRepeatingTimer<
  std::chrono::duration<double, std::ratio<1>>,
  uitsl::CoarseClock
>;

using CoarseRealFixedRateRepeatingTimer = uitsl::FixedRateRepeatingTimer<
  std::chrono::duration<double, std::ratio<1>>,
  uitsl::CoarseRealClock
>;


} // namespace uitsl

#endif // #ifndef UITSL_COUNTDOWN_FIXEDRATEREPEATINGTIMER_HPP_INCLUDE
