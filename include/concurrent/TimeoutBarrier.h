#pragma once

#include <condition_variable>
#include <thread>

#include <mpi.h>

#include "../utility/numeric_cast.h"
#include "../polyfill/barrier.h"
#include "../utility/CountdownTimer.h"

#include "../distributed/mpi_utils.h"

namespace uit {

namespace internal {

class IBarrierRequest {

  MPI_Request request;

public:

  IBarrierRequest(
    MPI_Comm comm=MPI_COMM_WORLD
  ) {
    verify(MPI_Ibarrier(
      comm,
      &request
    ));
  }

  bool IsComplete() {
    int flag{};
    verify(MPI_Test(
      &request,
      &flag,
      MPI_STATUS_IGNORE
    ));
    return flag;
  }

};

}

template<typename Timer_T=CountdownTimer<>>
class TimeoutBarrier {

  internal::IBarrierRequest proc_barrier;

public:

  TimeoutBarrier(
    const Timer_T& timer=Timer_T{},
    MPI_Comm comm=MPI_COMM_WORLD
  ) : proc_barrier(comm) {

    while (!proc_barrier.IsComplete() && !timer.IsComplete());

  }

  template<typename T>
  TimeoutBarrier(
    const Timer_T& timer,
    std::barrier<T>& thread_barrier,
    MPI_Comm comm=MPI_COMM_WORLD
  ) : proc_barrier(comm) {

    thread_barrier.arrive_and_wait();

    while (!proc_barrier.IsComplete() && !timer.IsComplete());

  }

};

}
