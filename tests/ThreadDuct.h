#include <thread>
#include <unordered_set>

#include "mpi.h"

#include "../source/config_utils.h"
#include "../source/Gatherer.h"
#include "../source/CircularIndex.h"
#include "../source/ThreadTeam.h"
#include "../source/Mesh.h"
#include "../source/latch.h"

#include "../source/benchmark_utils.h"
#include "../source/mpi_utils.h"
#include "../source/pipe_utils.h"
#include "../source/numeric_cast.h"
#include "../source/mesh_utils.h"
#include "../source/math_utils.h"

#define MSG_T int
#define num_nodes 4

Gatherer<MSG_T> gatherer(MPI_INT);

void do_work(io_bundle_t<MSG_T> bundle, const size_t node_id) {

  bundle.outputs[0].GetOutput().MaybePut(node_id);

  static latch latch{numeric_cast<std::ptrdiff_t>(num_nodes)};

  for (size_t rep = 0; rep < 100; ++rep) {
    bundle.outputs[0].GetOutput().MaybePut(node_id);
    do_not_optimize(
      bundle.inputs[0].GetInput().GetCurrent()
    );
  }

  const MSG_T res = bundle.inputs[0].GetInput().GetCurrent();

  assert( res == circular_index(node_id, num_nodes, -1) );

  gatherer.Put(res);

}


int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  ThreadTeam team;

  Mesh mesh{
    make_ring_mesh<MSG_T>(num_nodes),
    assign_segregated<thread_id_t>()
  };

  for (size_t node_id = 0; node_id < mesh.GetSize(); ++node_id) {
    auto & node = mesh[node_id];
    team.Add([=](){ do_work(node, node_id); });
  }

  team.Join();

  auto res = gatherer.Gather();

  if (res) {
    assert(
      std::unordered_set(
        std::begin(*res),
        std::end(*res)
      ).size() == res->size()
    );

    for (auto & val : *res) std::cout << "v: " << val << std::endl;

  }

  MPI_Finalize();

  return 0;
}
