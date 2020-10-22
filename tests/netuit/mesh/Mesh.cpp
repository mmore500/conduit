#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/mpi/MpiGuard.hpp"

#include "uit/setup/ImplSpec.hpp"

#include "netuit/arrange/ProConTopologyFactory.hpp"
#include "netuit/arrange/RingTopologyFactory.hpp"
#include "netuit/mesh/Mesh.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test Mesh") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;

  netuit::Mesh<Spec> mesh{netuit::RingTopologyFactory{}(100)};

  REQUIRE( mesh.GetNodeCount() == 100 );
  REQUIRE( mesh.GetEdgeCount() == 100 );
  REQUIRE( mesh.GetSubmesh().size() == 100 );

}

TEST_CASE("Test with ProConTopologyFactory") {

  using Spec = uit::ImplSpec<char>;

  netuit::Mesh<Spec> mesh{netuit::ProConTopologyFactory{}(100)};

  REQUIRE( mesh.GetNodeCount() == 100 );
  REQUIRE( mesh.GetEdgeCount() == 50 );
  REQUIRE( mesh.GetSubmesh().size() == 100 );

}

// TODO add tests with more TopologyFactories
// TODO add tests with no-connection nodes
