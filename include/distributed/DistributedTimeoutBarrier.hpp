#pragma once

#include <thread>

#include <mpi.h>

#include "../utility/CountdownTimer.hpp"
#include "../parallel/ThreadIbarrier.hpp"

#include "IbarrierRequest.hpp"
#include "mpi_utils.hpp"

namespace uit {

/**
 * Block until all processes reach the barrier or a timeout is exceeded.
 *
 * @tparam Timer_T class to manage timeout check.
 */
// TODO is Ibarrier request leaked?
template<typename Timer_T=uit::CountdownTimer<>>
class DistributedTimeoutBarrier {

  /// manages state of MPI Ibarrier call
  uit::IbarrierRequest proc_barrier;

public:

  /**
  * Blocks until all processes have reached barrier or a timeout is exceeded.
   */
  DistributedTimeoutBarrier(
    const Timer_T& timer=Timer_T{},
    MPI_Comm comm=MPI_COMM_WORLD
  ) : proc_barrier(comm) {

    while (!proc_barrier.IsComplete() && !timer.IsComplete());

  }

};

} // end namespace uit
