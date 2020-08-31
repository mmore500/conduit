#include <fstream>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "topology/DyadicTopologyFactory.hpp"
#include "factory_common.hpp"

TEST_CASE("Test DyadicTopologyFactory") {

  // TODO flesh out stub test
  uit::DyadicTopologyFactory{}(100);

  for (size_t i = 0; i < 150; ++i) {
    REQUIRE( uit::DyadicTopologyFactory{}(i).GetSize() == i );
  }

  for (const auto& node : uit::DyadicTopologyFactory{}(100)) {
    REQUIRE( node.GetNumInputs() == 1);
    REQUIRE( node.GetNumOutputs() == 1);
  }

}

TEST_CASE("Test file output") {

  // TODO: implement matchers to move this require further in?
  // https://github.com/catchorg/Catch2/blob/master/docs/matchers.md
  REQUIRE(test_all_adj(uit::DyadicTopologyFactory{}));

}