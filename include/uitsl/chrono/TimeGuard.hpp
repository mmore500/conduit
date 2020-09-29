#pragma once
#ifndef UITSL_CHRONO_TIMEGUARD_HPP_INCLUDE
#define UITSL_CHRONO_TIMEGUARD_HPP_INCLUDE

#include <chrono>

namespace uitsl {

template<typename DurationType>
class TimeGuard {

  DurationType &dest;

  const std::chrono::time_point<std::chrono::steady_clock> start;

public:

  TimeGuard(DurationType &dest_)
  : dest{dest_}
  , start{std::chrono::steady_clock::now()}
  { ; }

  ~TimeGuard() {
    dest = std::chrono::duration_cast<DurationType>(
      std::chrono::steady_clock::now() - start
    );
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_CHRONO_TIMEGUARD_HPP_INCLUDE
