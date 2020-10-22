#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/topology/LoopTopologyFactory.hpp"

#include "NetworkXTester.hpp"

TEST_CASE("Test LoopTopologyFactory") {
  REQUIRE(test_all_adj(uit::LoopTopologyFactory{}));
}
