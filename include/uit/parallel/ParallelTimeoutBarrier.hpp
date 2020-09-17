#pragma once

#include <thread>

#include <mpi.h>

#include "../countdown/Timer.hpp"

#include "ThreadIbarrier.hpp"

namespace uit {

/**
 * Block until all threads reach the barrier or a timeout is exceeded.
 *
 * @tparam Timer_T class to manage timeout check.
 */
template<typename Timer_T=uit::Timer<>>
class ParallelTimeoutBarrier {

public:

  /**
  * Blocks until all threads have reached barrier, or timer expires.
   */
  ParallelTimeoutBarrier(
    const uit::ThreadIbarrier& thread_barrier,
    const Timer_T& timer=Timer_T{}
  ) {

    while (!thread_barrier.IsComplete() && !timer.IsComplete());

  }

};

} // namespace uit
