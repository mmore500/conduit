#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "mesh/Mesh.h"
#include "conduit/ImplSpec.h"
#include "distributed/MPIGuard.h"
#include "topology/RingTopologyFactory.h"

const uit::MPIGuard guard;

TEST_CASE("Test Mesh") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;

  uit::Mesh<Spec> mesh{uit::RingTopologyFactory{}(100)};

  REQUIRE( mesh.GetNodeCount() == 100 );
  REQUIRE( mesh.GetEdgeCount() == 100 );
  REQUIRE( mesh.GetSubmesh().size() == 100 );

}
