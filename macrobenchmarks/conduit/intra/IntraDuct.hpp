#include <thread>

#include <mpi.h>

#include "uit/conduit/Conduit.hpp"
#include "uit/conduit/config.hpp"
#include "uit/concurrent/Gatherer.hpp"
#include "uit/mpi/mpi_utils.hpp"
#include "uit/mesh/Mesh.hpp"
#include "uit/parallel/ThreadTeam.hpp"
#include "uit/parallel/thread_utils.hpp"
#include "uit/polyfill/latch.hpp"
#include "uit/topology/RingTopologyFactory.hpp"
#include "uit/debug/benchmark_utils.hpp"
#include "uit/nonce/CircularIndex.hpp"
#include "uit/debug/safe_cast.hpp"
#include "uit/utility/TimeGuard.hpp"

#define MESSAGE_T int

using Spec = uit::ImplSpec<MESSAGE_T, DEFAULT_BUFFER, ImplSel>;

void do_work(
  uit::Mesh<Spec>::submesh_t submesh,
  std::latch & latch,
  uit::Gatherer<MESSAGE_T> & gatherer
) {

  std::chrono::milliseconds duration; { const uit::TimeGuard guard{duration};

  auto optional_input = submesh.front().GetInputOrNullopt(0);
  auto optional_output = submesh.front().GetOutputOrNullopt(0);

  if (optional_output) optional_output->TryPut(uit::get_thread_id());

  latch.arrive_and_wait();

  for (size_t rep = 0; rep < 1e7; ++rep) {
    if (optional_output) optional_output->TryPut(uit::get_thread_id());
    if (optional_input) uit::do_not_optimize(
      optional_input->JumpGet()
    );
  }

  } // close TimeGuard

  gatherer.Put(duration.count());

}

void profile_thread_count(const size_t num_threads) {

  uit::ThreadTeam team;

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>()
  };

  uit::Gatherer<MESSAGE_T> gatherer(MPI_INT);

  std::chrono::milliseconds duration; { const uit::TimeGuard guard{duration};

  std::latch latch{uit::safe_cast<std::ptrdiff_t>(num_threads)};
  for (uit::thread_id_t i = 0; i < num_threads; ++i) {
    team.Add(
      [i, &latch, &gatherer, &mesh](){
        do_work(mesh.GetSubmesh(i), latch, gatherer);
      }
    );
  }

  team.Join();

  } // close TimeGuard

  auto res = gatherer.Gather();

  if (res) {

    std::cout << "threads: " << num_threads << std::endl;

    std::cout << "mean milliseconds:" << std::accumulate(
        std::begin(*res),
        std::end(*res),
        0.0
      ) / std::size(*res) << std::endl;

    std::cout << "net milliseconds:" << duration.count() << std::endl;

  }


}

int main(int argc, char* argv[]) {

  int provided;
  UIT_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
  emp_assert(provided >= MPI_THREAD_FUNNELED);

  // TODO
  // for (size_t threads = 1; threads <= uit::get_nproc(); threads*=2) {
  //   profile_thread_count(threads);
  // }

  MPI_Finalize();

  return 0;
}
