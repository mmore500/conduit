#include <thread>

#include "mpi.h"

#include "../source/CircularIndex.h"
#include "../source/ThreadTeam.h"
#include "../source/Mesh.h"
#include "../source/latch.h"
#include "../source/TimeGuard.h"

#include "../source/mpi_utils.h"
#include "../source/pipe_utils.h"
#include "../source/numeric_cast.h"
#include "../source/mesh_utils.h"
#include "../source/thread_utils.h"
#include "../source/benchmark_utils.h"

#define MESSAGE_T int

void do_work(
  io_bundle_t<MESSAGE_T> bundle,
  latch & latch
) {

  std::chrono::milliseconds duration; { const TimeGuard guard{duration};

  latch.arrive_and_wait();

  do_compute_work(1e7);

  } // close TimeGuard

}

void profile_thread_count(const size_t num_threads) {

  ThreadTeam team;

  Mesh mesh{
    make_loop_mesh<MESSAGE_T>(num_threads),
    assign_segregated<thread_id_t>()
  };

  std::chrono::milliseconds duration; { const TimeGuard guard{duration};

  latch latch{numeric_cast<std::ptrdiff_t>(num_threads)};
  for (auto & node : mesh) {
    team.Add(
      [node, &latch](){ do_work(node, latch); }
    );
  }

  team.Join();

  } // close TimeGuard


  std::cout << "t: " << duration.count() << std::endl;

}

int main(int argc, char* argv[]) {

  verify(MPI_Init(&argc, &argv));

  for (size_t threads = 1; threads <= get_nproc(); ++threads) {
    profile_thread_count(threads);
  }

  MPI_Finalize();

  return 0;
}
