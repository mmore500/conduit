#include <thread>

#include <mpi.h>

#include "uit/conduit/ImplSpec.hpp"
#include "uit/mpi/mpi_utils.hpp"
#include "uit/mesh/Mesh.hpp"
#include "uit/polyfill/latch.hpp"
#include "uit/parallel/ThreadTeam.hpp"
#include "uit/parallel/thread_utils.hpp"
#include "uit/topology/LoopTopologyFactory.hpp"
#include "uit/debug/benchmark_utils.hpp"
#include "uit/nonce/CircularIndex.hpp"
#include "uit/debug/safe_cast.hpp"
#include "uit/utility/TimeGuard.hpp"

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

  std::latch latch{uit::safe_cast<std::ptrdiff_t>(num_threads)};
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
