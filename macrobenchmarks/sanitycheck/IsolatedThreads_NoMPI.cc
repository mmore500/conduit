#include <thread>

#include "mpi.h"

#include "utility/CircularIndex.h"
#include "parallel/ThreadTeam.h"
#include "mesh/Mesh.h"
#include "polyfill/latch.h"
#include "utility/TimeGuard.h"

#include "distributed/mpi_utils.h"
#include "utility/numeric_cast.h"
#include "topology/LoopTopologyFactory.h"
#include "parallel/thread_utils.h"
#include "utility/benchmark_utils.h"
#include "conduit/ImplSpec.h"

#define MESSAGE_T int

void do_work(std::latch & latch) {

  std::chrono::milliseconds duration; { const uit::TimeGuard guard{duration};

  latch.arrive_and_wait();

  uit::do_compute_work(1e7);

  } // close TimeGuard

}

void profile_thread_count(const size_t num_threads) {

  uit::ThreadTeam team;

  std::chrono::milliseconds duration; { const uit::TimeGuard guard{duration};

  std::latch latch{uit::numeric_cast<std::ptrdiff_t>(num_threads)};
  for (size_t i = 0; i < num_threads; ++i) {
    team.Add( [&latch](){ do_work(latch); } );
  }

  team.Join();

  } // close TimeGuard

  std::cout << "t: " << duration.count() << std::endl;

}

int main(int argc, char* argv[]) {

  for (size_t threads = 1; threads <= uit::get_nproc(); threads *= 2) {
    profile_thread_count(threads);
  }

  return 0;
}
