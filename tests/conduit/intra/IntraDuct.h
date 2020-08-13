#include <thread>
#include <unordered_set>
#include <cassert>
#include <ratio>

#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "../../MultiprocessReporter.h"

#include "conduit/config.h"
#include "utility/CircularIndex.h"
#include "mesh/Mesh.h"

#include "distributed/MPIGuard.h"
#include "utility/numeric_cast.h"
#include "utility/safe_compare.h"
#include "topology/RingTopologyFactory.h"
#include "utility/math_utils.h"
#include "utility/assign_utils.h"

#define MSG_T int
#define num_nodes 4

using Spec = uit::ImplSpec<MSG_T, DEFAULT_BUFFER, ImplSel>;

const uit::MPIGuard guard;

TEST_CASE("Test IntraDuct Connectivity") {

  uit::Mesh<Spec> mesh{ uit::RingTopologyFactory{}(num_nodes) };

  for (auto & node : mesh.GetSubmesh()) node.GetOutput(0).MaybePut(
    node.GetNodeID()
  );

  for (auto & node : mesh.GetSubmesh()) {
    const MSG_T res = node.GetInput(0).GetCurrent();
    REQUIRE(
      uit::safe_equal(res, uit::circular_index(node.GetNodeID(), num_nodes, -1))
    );
  }

}

TEST_CASE("Test IntraDuct Validity") {

  uit::Mesh<Spec> mesh{ uit::RingTopologyFactory{}(num_nodes) };

  std::unordered_map<size_t, MSG_T> last;
  for (MSG_T msg = 0; msg < 10 * std::kilo{}.num; ++msg) {
    for (auto & node : mesh.GetSubmesh()) {
      node.GetOutput(0).MaybePut(msg);
      const MSG_T current = node.GetInput(0).GetCurrent();
      REQUIRE( current >= 0 );
      REQUIRE( current < 10 * std::kilo{}.num );
      REQUIRE( last[node.GetNodeID()] <= current );
      last[node.GetNodeID()] = current;
    }
  }

  for (size_t i = 0; i < 10 * std::kilo{}.num; ++i) {
    for (auto & node : mesh.GetSubmesh()) {
      REQUIRE( node.GetInput(0).GetCurrent() >= 0 );
      REQUIRE( node.GetInput(0).GetCurrent() == node.GetInput(0).GetCurrent() );
    }
  }

}
