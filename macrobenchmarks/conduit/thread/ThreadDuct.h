#include <thread>

#include "mpi.h"

#include "conduit/config.h"
#include "concurrent/Gatherer.h"
#include "utility/CircularIndex.h"
#include "parallel/ThreadTeam.h"
#include "mesh/Mesh.h"
#include "polyfill/latch.h"
#include "utility/TimeGuard.h"

#include "distributed/mpi_utils.h"
#include "conduit/Conduit.h"
#include "conduit/ImplSpec.h"
#include "utility/numeric_cast.h"
#include "topology/RingTopologyFactory.h"
#include "parallel/thread_utils.h"
#include "utility/benchmark_utils.h"

#define MESSAGE_T int

using Spec = uit::ImplSpec<MESSAGE_T, DEFAULT_BUFFER, ImplSel>;

void do_work(
  uit::Mesh<Spec>::submesh_t submesh,
  std::latch& latch,
  uit::Gatherer<MESSAGE_T>& gatherer
) {

  std::chrono::milliseconds duration; { const uit::TimeGuard guard{duration};

  auto optional_input = submesh.front().GetInputOrNullopt(0);
  auto optional_output = submesh.front().GetOutputOrNullopt(0);

  if (optional_output) optional_output->MaybePut(uit::get_thread_id());

  latch.arrive_and_wait();

  for (size_t rep = 0; rep < 1e7; ++rep) {
    if (optional_output) optional_output->MaybePut(uit::get_thread_id());
    if (optional_input) uit::do_not_optimize(
      optional_input->GetCurrent()
    );
  }

  } // close TimeGuard

  gatherer.Put(duration.count());

}

void profile_thread_count(const size_t num_threads) {

  uit::ThreadTeam team;

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  uit::Gatherer<MESSAGE_T> gatherer(MPI_INT);

  std::chrono::milliseconds duration; { const uit::TimeGuard guard{duration};

  std::latch latch{uit::numeric_cast<std::ptrdiff_t>(num_threads)};
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
      ) / std::size(*res) << std::endl;;

    std::cout << "net milliseconds:" << duration.count() << std::endl;

  }


}

int main(int argc, char* argv[]) {

  int provided;
  uit::verify(MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided));
  emp_assert(provided >= MPI_THREAD_FUNNELED);

  for (size_t threads = 1; threads <= uit::get_nproc(); threads*=2) {
    profile_thread_count(threads);
  }

  MPI_Finalize();

  return 0;
}
