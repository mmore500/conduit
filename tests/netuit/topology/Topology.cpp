
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/vector.hpp"

#include "netuit/arrange/CompleteTopologyFactory.hpp"
#include "netuit/arrange/DyadicTopologyFactory.hpp"
#include "netuit/arrange/EmptyTopologyFactory.hpp"
#include "netuit/arrange/ToroidalTopologyFactory.hpp"
#include "netuit/topology/Topology.hpp"

TEST_CASE("Test Topology", "[nproc:1]") {

  // TODO flesh out stub test
  netuit::Topology{};

}

TEST_CASE("Test AsCSR Null Topology", "[nproc:1]") {

  netuit::Topology topology{};
  const auto [x_adj, adjacency] = topology.AsCSR();

  REQUIRE( x_adj == emp::vector<int>{} );
  REQUIRE( adjacency == emp::vector<int>{} );

}

TEST_CASE("Test AsCSR CompleteTopologyFactory", "[nproc:1]") {

  netuit::Topology topology = netuit::make_complete_topology( 3 );
  const auto [x_adj, adjacency] = topology.AsCSR();

  REQUIRE( x_adj == emp::vector<int>{ 0, 2, 4, 6 } );
  REQUIRE( adjacency == emp::vector<int>{ 1, 2, 0, 2, 0, 1 } );


}

TEST_CASE("Test AsCSR DyadicTopologyFactory", "[nproc:1]") {

  netuit::Topology topology = netuit::make_dyadic_topology( 3 );
  const auto [x_adj, adjacency] = topology.AsCSR();

  REQUIRE( x_adj == emp::vector<int>{ 0, 1, 2, 3 } );
  REQUIRE( adjacency == emp::vector<int>{ 1, 0, 2 } );


}

TEST_CASE("Test AsCSR Empty Topology", "[nproc:1]") {

  netuit::Topology topology = netuit::make_empty_topology( 3 );
  const auto [x_adj, adjacency] = topology.AsCSR();

  REQUIRE( x_adj == emp::vector<int>{ 0, 0, 0, 0 } );
  REQUIRE( adjacency == emp::vector<int>{} );

}

TEST_CASE("Test AsCSR ToroidalTopologyFactory", "[nproc:1]") {

  netuit::Topology topology = netuit::make_toroidal_topology( {3, 3} );
  const auto [x_adj, adjacency] = topology.AsCSR();

  /*
   * 0 1 2
   * 3 4 5
   * 6 7 8
   */

  REQUIRE( x_adj == emp::vector<int>{ 0, 4, 8, 12, 16, 20, 24, 28, 32, 36 } );
  REQUIRE( adjacency == emp::vector<int>{
    1, 2, 3, 6, // 0's neighbors
    2, 0, 4, 7, // 1's neighbors
    0, 1, 5, 8, // 2's neighbors
    4, 5, 6, 0, // 3's neighbors
    5, 3, 7, 1, // 4's neighbors
    3, 4, 8, 2, // 5's neighbors
    7, 8, 0, 3, // 6's neighbors
    8, 6, 1, 4, // 7's neighbors
    6, 7, 2, 5, // 8's neighbors
  } );


}

TEST_CASE("Test trivial Subtopology, ToroidalTopologyFactory", "[nproc:1]") {

  netuit::Topology topology = netuit::make_toroidal_topology( {3, 3} );
  netuit::Topology subtopo = topology.GetSubTopology(
    {0, 1, 2, 3, 4, 5, 6, 7, 8}
  );

  auto [x_adj, adjacency] = subtopo.AsCSR();

  /*
   * 0 1 2
   * 3 4 5
   * 6 7 8
   */

  REQUIRE( x_adj == emp::vector<int>{ 0, 4, 8, 12, 16, 20, 24, 28, 32, 36 } );
  REQUIRE( adjacency == emp::vector<int>{
    2, 1, 6, 3, // 0's neighbors
    0, 2, 7, 4, // 1's neighbors
    1, 0, 8, 5, // 2's neighbors
    5, 4, 0, 6, // 3's neighbors
    3, 5, 1, 7, // 4's neighbors
    4, 3, 2, 8, // 5's neighbors
    8, 7, 3, 0, // 6's neighbors
    6, 8, 4, 1, // 7's neighbors
    7, 6, 5, 2, // 8's neighbors
  } );

}

TEST_CASE("Test Subtopology, ToroidalTopologyFactory", "[nproc:1]") {

  netuit::Topology topology = netuit::make_toroidal_topology( {3, 3} );
  netuit::Topology subtopo = topology.GetSubTopology({0, 1, 2, 3, 4, 5});

  auto [x_adj, adjacency] = subtopo.AsCSR();

  REQUIRE( x_adj == emp::vector<int>{ 0, 3, 6, 9, 12, 15, 18 } );
  REQUIRE( adjacency == emp::vector<int>{
    2, 1, 3, // 0's neighbors
    0, 2, 4, // 1's neighbors
    1, 0, 5, // 2's neighbors
    5, 4, 0, // 3's neighbors
    3, 5, 1, // 4's neighbors
    4, 3, 2, // 5's neighbors
  } );


}
