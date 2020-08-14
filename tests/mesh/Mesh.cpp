#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/ImplSpec.hpp"
#include "distributed/MPIGuard.hpp"
#include "mesh/Mesh.hpp"
#include "topology/RingTopologyFactory.hpp"
#include "topology/ProConTopologyFactory.hpp"

const uit::MPIGuard guard;

TEST_CASE("Test Mesh") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;

  uit::Mesh<Spec> mesh{uit::RingTopologyFactory{}(100)};

  REQUIRE( mesh.GetNodeCount() == 100 );
  REQUIRE( mesh.GetEdgeCount() == 100 );
  REQUIRE( mesh.GetSubmesh().size() == 100 );

}

TEST_CASE("Test with ProConTopologyFactory") {

  using Spec = uit::ImplSpec<char>;

  uit::Mesh<Spec> mesh{uit::ProConTopologyFactory{}(100)};

  REQUIRE( mesh.GetNodeCount() == 100 );
  REQUIRE( mesh.GetEdgeCount() == 50 );
  REQUIRE( mesh.GetSubmesh().size() == 100 );

}

// TODO add tests with more TopologyFactories
// TODO add tests with no-connection nodes
