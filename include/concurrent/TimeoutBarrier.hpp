#pragma once

#include <condition_variable>
#include <thread>

#include <mpi.h>

#include "../distributed/IbarrierRequest.hpp"
#include "../distributed/mpi_utils.hpp"
#include "../utility/CountdownTimer.hpp"
#include "../parallel/ThreadIbarrier.hpp"

namespace uit {

/**
 * Block until all processes reach the barrier or a timeout is exceeded.
 *
 * @tparam Timer_T class to manage timeout check.
 */
// TODO is Ibarrier request leaked?
template<typename Timer_T=uit::CountdownTimer<>>
class TimeoutBarrier {

  /// manages state of MPI Ibarrier call
  // TODO this won't work properly with different thread counts per process
  IbarrierRequest proc_barrier;

public:

  /**
  * Blocks until all process have reached barrier,
  * or timer expires.
   */
  TimeoutBarrier(
    const Timer_T& timer=Timer_T{},
    MPI_Comm comm=MPI_COMM_WORLD
  ) : proc_barrier(comm) {

    while (!proc_barrier.IsComplete() && !timer.IsComplete());

  }

  /**
   * Blocks until all process and threads have reached barrier,
   * or timer expires.
   */
  TimeoutBarrier(
    const uit::ThreadIbarrier& thread_barrier,
    const Timer_T& timer=Timer_T{},
    MPI_Comm comm=MPI_COMM_WORLD
  ) : proc_barrier(comm) {

    while (
      !thread_barrier.IsComplete()
      && !proc_barrier.IsComplete()
      && !timer.IsComplete()
    );

  }

};

} // end namespace uit
