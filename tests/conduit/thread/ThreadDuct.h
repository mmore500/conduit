#include <thread>
#include <unordered_set>
#include <cassert>

#include "mpi.h"

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "../../MultiprocessReporter.h"

#include "conduit/config.h"
#include "concurrent/Gatherer.h"
#include "utility/CircularIndex.h"
#include "parallel/ThreadTeam.h"
#include "mesh/Mesh.h"
#include "polyfill/latch.h"

#include "utility/benchmark_utils.h"
#include "distributed/mpi_utils.h"
#include "distributed/MPIGuard.h"
#include "conduit/Conduit.h"
#include "conduit/ImplSpec.h"
#include "utility/numeric_cast.h"
#include "utility/safe_compare.h"
#include "topology/RingTopologyFactory.h"
#include "utility/math_utils.h"
#include "utility/assign_utils.h"

#define MSG_T int
#define num_nodes 4

using Spec = uit::ImplSpec<MSG_T, DEFAULT_BUFFER, ImplSel>;

const uit::MPIGuard guard;

uit::Gatherer<MSG_T> gatherer(MPI_INT);

void do_work(uit::MeshNode<Spec> node, const size_t node_id) {

  node.GetOutput(0).MaybePut(node_id);

  static std::latch sync_before{uit::numeric_cast<std::ptrdiff_t>(num_nodes)};
  sync_before.arrive_and_wait();

  for (size_t rep = 0; rep < 100; ++rep) node.GetOutput(0).MaybePut(node_id);

  static std::latch sync_after{uit::numeric_cast<std::ptrdiff_t>(num_nodes)};
  sync_after.arrive_and_wait();

  const MSG_T res = node.GetInput(0).GetCurrent();

  REQUIRE( uit::safe_equal(res, uit::circular_index(node_id, num_nodes, -1)) );

  gatherer.Put(res);

}


TEST_CASE("Test ThreadDuct") {

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
    REQUIRE(
      std::unordered_set(
        std::begin(*res),
        std::end(*res)
      ).size() == res->size()
    );

    for (auto & val : *res) std::cout << "v: " << val << std::endl;

  }

}
