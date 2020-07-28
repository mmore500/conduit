#pragma once

#include <algorithm>
#include <chrono>
#include <limits>

#include "chrono_utils.h"

class CountdownIterator {

  const size_t duration;

  size_t elapsed{};

public:
  CountdownIterator(
    const size_t duration_=std::numeric_limits<size_t>::max()
  ) : duration{duration_}
  { ; }

  bool IsComplete() const {
    return GetElapsed() >= duration;
  }

  void Step() { ++elapsed; }

  size_t GetElapsed() const {
    return elapsed;
  }

  size_t GetRemaining() const {
    return GetElapsed() > duration
      ? duration - GetElapsed()
      : 0
    ;
  }

};
