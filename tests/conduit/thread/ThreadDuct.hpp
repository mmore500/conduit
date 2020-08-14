#include <ratio>
#include <thread>
#include <unordered_set>

#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/config.hpp"
#include "conduit/Conduit.hpp"
#include "conduit/ImplSpec.hpp"
#include "concurrent/Gatherer.hpp"
#include "distributed/MPIGuard.hpp"
#include "distributed/mpi_utils.hpp"
#include "distributed/MultiprocessReporter.hpp"
#include "mesh/Mesh.hpp"
#include "parallel/ThreadTeam.hpp"
#include "polyfill/latch.hpp"
#include "topology/RingTopologyFactory.hpp"
#include "utility/assign_utils.hpp"
#include "utility/benchmark_utils.hpp"
#include "utility/CircularIndex.hpp"
#include "utility/math_utils.hpp"
#include "utility/numeric_cast.hpp"
#include "utility/safe_compare.hpp"

#define MSG_T int
#define num_nodes 4

using Spec = uit::ImplSpec<MSG_T, DEFAULT_BUFFER, ImplSel>;

const uit::MPIGuard guard;

uit::Gatherer<MSG_T> gatherer(MPI_INT);

void check_connectivity(uit::MeshNode<Spec> node, const size_t node_id) {

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

TEST_CASE("Test ThreadDuct Connectivity") {

  uit::ThreadTeam team;

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_nodes),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  for (size_t i = 0; i < num_nodes; ++i) {
    team.Add([=](){ check_connectivity(mesh.GetSubmesh(i)[0], i); });
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

void check_validity(uit::MeshNode<Spec> node, const size_t node_id) {

  static std::latch sync_before{uit::numeric_cast<std::ptrdiff_t>(num_nodes)};
  sync_before.arrive_and_wait();

  MSG_T last{};
  for (MSG_T msg = 0; msg < 10 * std::kilo{}.num; ++msg) {
    node.GetOutput(0).MaybePut(msg);
    const MSG_T current = node.GetInput(0).GetCurrent();
    REQUIRE( current >= 0 );
    REQUIRE( current < 10 * std::kilo{}.num );
    REQUIRE( last <= current );
    last = current;
  }

  // all puts must be complete for next part of the test
  static std::latch sync_after{uit::numeric_cast<std::ptrdiff_t>(num_nodes)};
  sync_after.arrive_and_wait();

  for (size_t i = 0; i < 10 * std::kilo{}.num; ++i) {
    REQUIRE( node.GetInput(0).GetCurrent() >= 0 );
    REQUIRE( node.GetInput(0).GetCurrent() == node.GetInput(0).GetCurrent() );
  }

}

TEST_CASE("Test ThreadDuct Validity") {

  uit::ThreadTeam team;

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_nodes),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  for (size_t i = 0; i < num_nodes; ++i) {
    team.Add([=](){ check_validity(mesh.GetSubmesh(i)[0], i); });
  }

  team.Join();

}
