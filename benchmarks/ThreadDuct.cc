#include <thread>

#include "mpi.h"

#include "../source/Gatherer.h"
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

#define MESSAGE_T int

void do_work(
  io_bundle_t<MESSAGE_T> bundle,
  latch & latch,
  Gatherer<MESSAGE_T> & gatherer
) {

  std::chrono::milliseconds duration; { const TimeGuard guard{duration};

  bundle.outputs[0].GetOutput().MaybePut(get_thread_id());

  latch.arrive_and_wait();

  for (size_t rep = 0; rep < 10000000; ++rep) {
    bundle.outputs[0].GetOutput().MaybePut(get_thread_id());
    const volatile auto do_not_optimize{
      bundle.inputs[0].GetInput().GetCurrent()
    };
  }

  } // close TimeGuard

  gatherer.Put(duration.count());

}

void profile_thread_count(const size_t num_threads) {

  ThreadTeam team;

  Mesh mesh{
    make_ring_mesh<MESSAGE_T>(num_threads),
    assign_segregated<thread_id_t>()
  };

  Gatherer<MESSAGE_T> gatherer(MPI_INT);

  std::chrono::milliseconds duration; { const TimeGuard guard{duration};

  latch latch{numeric_cast<std::ptrdiff_t>(num_threads)};
  for (auto & node : mesh) {
    team.Add(
      [node, &latch, &gatherer](){ do_work(node, latch, gatherer); }
    );
  }

  team.Join();

  } // close TimeGuard

  auto res = gatherer.Gather();


  if (res) {

    std::cout << "threads: " << num_threads << std::endl;

    for (auto & val : *res) std::cout << "v: " << val << std::endl;

    std::cout << "t: " << duration.count() << std::endl;

    std::cout << std::endl;

  }


}

int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  for (size_t threads = 1; threads < get_nproc(); ++threads) {
    profile_thread_count(threads);
  }

  MPI_Finalize();

  return 0;
}
