#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/ImplSpec.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uit/mesh/Mesh.hpp"
#include "uit/topology/RingTopologyFactory.hpp"
#include "uit/topology/ProConTopologyFactory.hpp"

const uitsl::MpiGuard guard;

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
