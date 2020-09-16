#pragma once

#include <algorithm>
#include <chrono>
#include <stddef.h>

#include "chrono_utils.hpp"
#include "CountdownWrapper.hpp"

namespace uit {

template<
  typename Duration_T=std::chrono::duration<double, std::ratio<1>>,
  typename Clock_T=std::chrono::steady_clock
>
class CountdownTimer {

  const Duration_T duration;

  const std::chrono::time_point<Clock_T> start{ Clock_T::now() };

  const size_t refresh_freq;
  mutable size_t refresh_counter{0};

public:

  using iterator = uit::CountdownWrapper<CountdownTimer>;
  using elapsed_t = Duration_T;

  CountdownTimer(
    const Duration_T& duration_=infinite_duration,
    const size_t refresh_freq=1
  ) : duration(duration_)
  , refresh_freq(refresh_freq)
  { ; }

  bool IsComplete() const {
    if (refresh_counter == std::numeric_limits<size_t>::max()) return true;
    else {
      const bool res = refresh_counter ? false : duration <= GetElapsed();
      refresh_counter = res
        ? std::numeric_limits<size_t>::max()
        : (refresh_counter + 1) % refresh_freq
      ;
      return res;
    }
  }

  Duration_T GetElapsed() const {
    return std::chrono::duration_cast<Duration_T>(
      Clock_T::now() - start
    );
  }

  Duration_T GetRemaining() const {
    return std::max(
      duration - GetElapsed(),
      Duration_T{0}
    );
  }

  double GetFractionComplete() const {
    return (
      static_cast<double>( GetElapsed().count() )
      / static_cast<double>( duration.count() )
    );
  }

  CountdownTimer& operator++() { return *this; }

  iterator begin() { return { *this }; }

  iterator end() { return {}; }

};

} // namespace uit
