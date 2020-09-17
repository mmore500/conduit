#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/audited_routines.hpp"
#include "uitsl/mpi/MpiMultithreadGuard.hpp"
#include "uitsl/parallel/ThreadTeam.hpp"

const uit::MpiMultithreadGuard guard;

TEST_CASE("Test MpiMultithreadGuard") {

  uit::ThreadTeam team;
  for (int thread = 0; thread < 4; ++thread) {
    team.Add([thread](){

      const int proc_id = uit::get_proc_id();
      // 0 <-> 1, 2 <-> 3, etc.
      const int target_rank = (
        proc_id
        // in odd case, last process needs to connect to itself
        + ( proc_id % 2 == 0 && proc_id < uit::get_nprocs() - 1 )
        - ( proc_id % 2 == 1 )
      );

      const int sendbuf{ thread };
      int recvbuf{};

      UIT_Sendrecv(
        &sendbuf, // sendbuf: initial address of send buffer
        1, // sendcount: number of elements in send buffer
        MPI_INT, // sendtype: type of elements in send buffer
        target_rank, // dest: rank of destination
        thread, // sendtag: send tag
        &recvbuf, // recvbuf: initial address of receive buffer
        1, // recvcount: number of elements in receive buffer
        MPI_INT, // recvtype: type of elements in receive buffer
        target_rank, // source: rank of source
        thread, // recvtag: receive tag
        MPI_COMM_WORLD, // comm: communicator
        MPI_STATUS_IGNORE // status: status object for receive operation
      );

      REQUIRE( recvbuf == thread );

    });

  }

  team.Join();

  MPI_Barrier(MPI_COMM_WORLD);
  REQUIRE(true);

}
