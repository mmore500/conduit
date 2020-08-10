#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "topology/ProConTopologyFactory.h"

TEST_CASE("Test ProConTopologyFactory") {

  // TODO flesh out stub test
  uit::ProConTopologyFactory{}(100);

  for (size_t i = 0; i < 150; ++i) {
    REQUIRE( uit::ProConTopologyFactory{}(i).size() == i );
  }

  for (const auto& node : uit::ProConTopologyFactory{}(100)) {
    REQUIRE( node.GetNumInputs() + node.GetNumOutputs() == 1);
    REQUIRE( node.HasInputs() != node.HasOutputs() );
  }

}
