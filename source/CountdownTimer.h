#pragma once

#include <algorithm>
#include <chrono>

#include "chrono_utils.h"

template<typename Duration_T>
class CountdownTimer {

  const Duration_T duration;

  const std::chrono::time_point<std::chrono::steady_clock> start{
    std::chrono::steady_clock::now()
  };

public:
  CountdownTimer(
    const Duration_T& duration_=infinite_duration
  ) : duration{duration_}
  { ; }

  bool IsComplete() const {
    return duration <= GetElapsed();
  }

  Duration_T GetElapsed() const {
    return std::chrono::duration_cast<Duration_T>(
      std::chrono::steady_clock::now() - start
    );
  }

  Duration_T GetRemaining() const {
    return std::max(
      duration - GetElapsed(),
      Duration_T{0}
    );
  }

};
