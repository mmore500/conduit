#include <cassert>
#include <ratio>
#include <thread>
#include <unordered_set>

#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/debug/safe_cast.hpp"
#include "uitsl/debug/safe_compare.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/math/math_utils.hpp"
#include "uitsl/nonce/CircularIndex.hpp"
#include "uitsl/utility/assign_utils.hpp"

#include "uit/setup/ImplSpec.hpp"

#include "netuit/mesh/Mesh.hpp"
#include "netuit/topology/RingTopologyFactory.hpp"
#include "netuit/topology/DyadicTopologyFactory.hpp"

#define REPEAT for (size_t rep = 0; rep < std::deca::num; ++rep)

#define num_nodes 4

using Spec = uit::ImplSpec<MSG_T, ImplSel>;

const uitsl::MpiGuard guard;

TEST_CASE("Is initial Get() result value-intialized?") { REPEAT {

  netuit::Mesh<Spec> mesh{ netuit::RingTopologyFactory{}(num_nodes) };

  for (auto & node : mesh.GetSubmesh()) {
    REQUIRE( node.GetInput(0).Get() == MSG_T{} );
    REQUIRE( node.GetInput(0).JumpGet() == MSG_T{} );
  }

} }
