#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/arrange/LoopTopologyFactory.hpp"

#include "NetworkXTester.hpp"

TEST_CASE("Test LoopTopologyFactory") {
  REQUIRE(test_all_adj(netuit::LoopTopologyFactory{}));
}
