#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "topology/LoopTopologyFactory.h"

TEST_CASE("Test LoopTopologyFactory") {

  // TODO flesh out stub test
  uit::LoopTopologyFactory{}(100);

  for (size_t i = 0; i < 150; ++i) {
    REQUIRE( uit::LoopTopologyFactory{}(i).size() == i );
  }

  for (const auto& node : uit::LoopTopologyFactory{}(100)) {
    REQUIRE( node.GetNumInputs() == 1);
    REQUIRE( node.GetNumOutputs() == 1);
  }

}
