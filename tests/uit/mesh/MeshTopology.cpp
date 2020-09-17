#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/mpi/MpiGuard.hpp"
#include "uit/config/ImplSpec.hpp"
#include "uit/mesh/MeshTopology.hpp"
#include "uit/topology/RingTopologyFactory.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test MeshTopology") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;

  uit::internal::MeshTopology<Spec> mesh_topology{
    uit::RingTopologyFactory{}(100)
  };

  REQUIRE( mesh_topology.GetNodeCount() == 100 );
  REQUIRE( mesh_topology.GetEdgeCount() == 100 );

}
