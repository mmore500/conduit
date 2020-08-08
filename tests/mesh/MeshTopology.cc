#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "topology/RingTopologyFactory.h"
#include "distributed/MPIGuard.h"
#include "mesh/MeshTopology.h"
#include "conduit/ImplSpec.h"

const uit::MPIGuard guard;

TEST_CASE("Test MeshTopology") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;

  uit::internal::MeshTopology<Spec> mesh_topology{
    uit::RingTopologyFactory{}(100)
  };

  REQUIRE( mesh_topology.GetNodeCount() == 100 );
  REQUIRE( mesh_topology.GetEdgeCount() == 100 );

}
