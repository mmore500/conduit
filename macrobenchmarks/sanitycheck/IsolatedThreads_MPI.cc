#include <thread>

#include "mpi.h"

#include "utility/CircularIndex.h"
#include "parallel/ThreadTeam.h"
#include "mesh/Mesh.h"
#include "polyfill/latch.h"
#include "utility/TimeGuard.h"

#include "distributed/mpi_utils.h"
#include "conduit/pipe_utils.h"
#include "utility/numeric_cast.h"
#include "mesh/mesh_utils.h"
#include "parallel/thread_utils.h"
#include "utility/benchmark_utils.h"

#define MESSAGE_T int

void do_work(
  uit::io_bundle_t<MESSAGE_T> bundle,
  std::latch & latch
) {

  std::chrono::milliseconds duration; { const uit::TimeGuard guard{duration};

  latch.arrive_and_wait();

  uit::do_compute_work(1e7);

  } // close TimeGuard

}

void profile_thread_count(const size_t num_threads) {

  uit::ThreadTeam team;

  uit::Mesh mesh{
    uit::make_loop_mesh<MESSAGE_T>(num_threads),
    uit::assign_segregated<uit::thread_id_t>()
  };

  std::chrono::milliseconds duration; { const uit::TimeGuard guard{duration};

  std::latch latch{uit::numeric_cast<std::ptrdiff_t>(num_threads)};
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

  int provided;
  uit::verify(MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided));
  emp_assert(provided >= MPI_THREAD_FUNNELED);

  for (size_t threads = 1; threads <= uit::get_nproc(); threads *= 2) {
    profile_thread_count(threads);
  }

  MPI_Finalize();

  return 0;
}
