#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "topology/RingTopologyFactory.hpp"
#include "distributed/MPIGuard.hpp"
#include "mesh/MeshTopology.hpp"
#include "conduit/ImplSpec.hpp"

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
