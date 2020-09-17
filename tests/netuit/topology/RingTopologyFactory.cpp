#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/topology/RingTopologyFactory.hpp"

TEST_CASE("Test RingTopologyFactory") {

  // TODO flesh out stub test
  netuit::RingTopologyFactory{}(100);

  for (size_t i = 0; i < 150; ++i) {
    REQUIRE( netuit::RingTopologyFactory{}(i).size() == i );
  }

  for (const auto& node : netuit::RingTopologyFactory{}(100)) {
    REQUIRE( node.GetNumInputs() == 1);
    REQUIRE( node.GetNumOutputs() == 1);
  }


}
