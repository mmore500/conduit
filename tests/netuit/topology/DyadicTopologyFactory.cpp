#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/topology/DyadicTopologyFactory.hpp"

TEST_CASE("Test DyadicTopologyFactory") {

  // TODO flesh out stub test
  netuit::DyadicTopologyFactory{}(100);

  for (size_t i = 0; i < 150; ++i) {
    REQUIRE( netuit::DyadicTopologyFactory{}(i).size() == i );
  }

  for (const auto& node : netuit::DyadicTopologyFactory{}(100)) {
    REQUIRE( node.GetNumInputs() == 1);
    REQUIRE( node.GetNumOutputs() == 1);
  }

}
