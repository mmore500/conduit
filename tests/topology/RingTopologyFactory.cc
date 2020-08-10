#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "topology/RingTopologyFactory.h"

TEST_CASE("Test RingTopologyFactory") {

  // TODO flesh out stub test
  uit::RingTopologyFactory{}(100);

  for (size_t i = 0; i < 150; ++i) {
    REQUIRE( uit::RingTopologyFactory{}(i).size() == i );
  }

  for (const auto& node : uit::RingTopologyFactory{}(100)) {
    REQUIRE( node.GetNumInputs() == 1);
    REQUIRE( node.GetNumOutputs() == 1);
  }


}
