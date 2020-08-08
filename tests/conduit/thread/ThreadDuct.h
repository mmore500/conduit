#include <thread>
#include <unordered_set>
#include <cassert>

#include "mpi.h"

#include "conduit/config.h"
#include "concurrent/Gatherer.h"
#include "utility/CircularIndex.h"
#include "parallel/ThreadTeam.h"
#include "mesh/Mesh.h"
#include "polyfill/latch.h"

#include "utility/benchmark_utils.h"
#include "distributed/mpi_utils.h"
#include "conduit/Conduit.h"
#include "conduit/ImplSpec.h"
#include "utility/numeric_cast.h"
#include "utility/safe_compare.h"
#include "topology/RingTopologyFactory.h"
#include "utility/math_utils.h"
#include "utility/assign_utils.h"

#define MSG_T int
#define num_nodes 4

using Spec = uit::ImplSpec<MSG_T>;

uit::Gatherer<MSG_T> gatherer(MPI_INT);

void do_work(uit::MeshNode<Spec> node, const size_t node_id) {

  node.GetOutput(0).MaybePut(node_id);

  static std::latch sync_before{uit::numeric_cast<std::ptrdiff_t>(num_nodes)};
  sync_before.arrive_and_wait();

  for (size_t rep = 0; rep < 100; ++rep) {
    node.GetOutput(0).MaybePut(node_id);
    uit::do_not_optimize(
      node.GetInput(0).GetCurrent()
    );
  }

  static std::latch sync_after{uit::numeric_cast<std::ptrdiff_t>(num_nodes)};
  sync_after.arrive_and_wait();

  const MSG_T res = node.GetInput(0).GetCurrent();

  assert(
    uit::safe_equal(res, uit::circular_index(node_id, num_nodes, -1))
    || [&](){
      std::cerr
        << res << ", " << uit::circular_index(node_id, num_nodes, -1)
        << std::endl;
      return false;
    }()
  );

  gatherer.Put(res);

}


int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  uit::ThreadTeam team;

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_nodes),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  for (size_t i = 0; i < num_nodes; ++i) {
    team.Add([=](){ do_work(mesh.GetSubmesh(i)[0], i); });
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
