#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/base/vector.h"

#include "netuit/arrange/CompleteTopologyFactory.hpp"
#include "netuit/arrange/DyadicTopologyFactory.hpp"
#include "netuit/arrange/EmptyTopologyFactory.hpp"
#include "netuit/arrange/ToroidalTopologyFactory.hpp"
#include "netuit/topology/Topology.hpp"

TEST_CASE("Test Topology") {

  // TODO flesh out stub test
  netuit::Topology{};

}

TEST_CASE("Test AsCSR Null Topology") {

  netuit::Topology topology{};
  const auto [x_adj, adjacency] = topology.AsCSR();

  REQUIRE( x_adj == emp::vector<int>{} );
  REQUIRE( adjacency == emp::vector<int>{} );

}

TEST_CASE("Test AsCSR CompleteTopologyFactory") {

  netuit::Topology topology = netuit::make_complete_topology( 3 );
  const auto [x_adj, adjacency] = topology.AsCSR();

  REQUIRE( x_adj == emp::vector<int>{ 0, 2, 4, 6 } );
  REQUIRE( adjacency == emp::vector<int>{ 1, 2, 0, 2, 0, 1 } );


}

TEST_CASE("Test AsCSR DyadicTopologyFactory") {

  netuit::Topology topology = netuit::make_dyadic_topology( 3 );
  const auto [x_adj, adjacency] = topology.AsCSR();

  REQUIRE( x_adj == emp::vector<int>{ 0, 1, 2, 3 } );
  REQUIRE( adjacency == emp::vector<int>{ 1, 0, 2 } );


}

TEST_CASE("Test AsCSR Empty Topology") {

  netuit::Topology topology = netuit::make_empty_topology( 3 );
  const auto [x_adj, adjacency] = topology.AsCSR();

  REQUIRE( x_adj == emp::vector<int>{ 0, 0, 0, 0 } );
  REQUIRE( adjacency == emp::vector<int>{} );

}

TEST_CASE("Test AsCSR ToroidalTopologyFactory") {

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
