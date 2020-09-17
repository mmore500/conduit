#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "uit/topology/DyadicTopologyFactory.hpp"

TEST_CASE("Test DyadicTopologyFactory") {

  // TODO flesh out stub test
  uit::DyadicTopologyFactory{}(100);

  for (size_t i = 0; i < 150; ++i) {
    REQUIRE( uit::DyadicTopologyFactory{}(i).size() == i );
  }

  for (const auto& node : uit::DyadicTopologyFactory{}(100)) {
    REQUIRE( node.GetNumInputs() == 1);
    REQUIRE( node.GetNumOutputs() == 1);
  }

}
