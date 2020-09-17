#pragma once

#include <thread>

#include <mpi.h>

#include "../countdown/Timer.hpp"

#include "ThreadIbarrier.hpp"

namespace uitsl {

/**
 * Block until all threads reach the barrier or a timeout is exceeded.
 *
 * @tparam Timer_T class to manage timeout check.
 */
template<typename Timer_T=uitsl::Timer<>>
class ParallelTimeoutBarrier {

public:

  /**
  * Blocks until all threads have reached barrier, or timer expires.
   */
  ParallelTimeoutBarrier(
    const uitsl::ThreadIbarrier& thread_barrier,
    const Timer_T& timer=Timer_T{}
  ) {

    while (!thread_barrier.IsComplete() && !timer.IsComplete());

  }

};

} // namespace uitsl
