#pragma once

#include <thread>

#include <mpi.h>

#include "../distributed/mpi_utils.hpp"
#include "../utility/CountdownTimer.hpp"
#include "../parallel/ParallelTimeoutBarrier.hpp"
#include "../parallel/ThreadIbarrier.hpp"

#include "ThreadSafeIbarrierRequest.hpp"

namespace uit {

/**
 * Block until all threads on all processes reach the barrier,
 * or a timeout is exceeded.
 *
 * @tparam Timer_T class to manage timeout check.
 */
// TODO is Ibarrier request leaked?
template<typename Timer_T=uit::CountdownTimer<>>
class ConcurrentTimeoutBarrier {

  /// manages state of MPI Ibarrier call
  // TODO this won't work properly with different thread counts per process
  uit::ThreadSafeIbarrierRequest proc_barrier;

public:

  /**
   * Blocks until all threads on all processes have reached barrier,
   * or a timeout expires.
   */
  ConcurrentTimeoutBarrier(
    const uit::ThreadIbarrier& thread_barrier,
    const Timer_T& timer=Timer_T{},
    const MPI_Comm comm=MPI_COMM_WORLD
  ) : proc_barrier(comm) {

    uit::ParallelTimeoutBarrier{thread_barrier, timer};
    while (!proc_barrier.IsComplete() && !timer.IsComplete());

  }

};

} // namespace uit
