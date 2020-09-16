#include <cassert>
#include <ratio>
#include <thread>
#include <unordered_set>

#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/config.hpp"
#include "uit/mpi/MpiGuard.hpp"
#include "uit/debug/MultiprocessReporter.hpp"
#include "uit/mesh/Mesh.hpp"
#include "uit/topology/RingTopologyFactory.hpp"
#include "uit/topology/DyadicTopologyFactory.hpp"
#include "uit/utility/assign_utils.hpp"
#include "uit/utility/CircularIndex.hpp"
#include "uit/utility/math_utils.hpp"
#include "uit/utility/safe_cast.hpp"
#include "uit/debug/safe_compare.hpp"

#define REPEAT for (size_t rep = 0; rep < std::deca{}.num; ++rep)

#define MSG_T int
#define num_nodes 4

using Spec = uit::ImplSpec<MSG_T, DEFAULT_BUFFER, ImplSel>;

const uit::MpiGuard guard;

TEST_CASE("Is initial Get() result value-intialized?") { REPEAT {

  uit::Mesh<Spec> mesh{ uit::RingTopologyFactory{}(num_nodes) };

  for (auto & node : mesh.GetSubmesh()) {
    REQUIRE( node.GetInput(0).Get() == MSG_T{} );
    REQUIRE( node.GetInput(0).JumpGet() == MSG_T{} );
  }

} }
