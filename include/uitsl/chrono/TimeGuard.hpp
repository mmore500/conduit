#pragma once

#include <chrono>

namespace uitsl {

template<typename DurationType, typename Clock=std::chrono::steady_clock>
class TimeGuard {

  DurationType &dest;

  const std::chrono::time_point<Clock> start;

public:

  TimeGuard(DurationType &dest_)
  : dest{dest_}
  , start{Clock::now()}
  { ; }

  ~TimeGuard() {
    dest = std::chrono::duration_cast<DurationType>( Clock::now() - start );
  }

};

} // namespace uitsl
