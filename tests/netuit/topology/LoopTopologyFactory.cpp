#include <fstream>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/topology/LoopTopologyFactory.hpp"

#include "FactoryCommon.hpp"

TEST_CASE("Test LoopTopologyFactory") {
  REQUIRE(test_all_adj(netuit::LoopTopologyFactory{}));
}
