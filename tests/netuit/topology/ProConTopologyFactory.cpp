#include <fstream>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/topology/ProConTopologyFactory.hpp"

#include "FactoryCommon.hpp"

/*
TEST_CASE("Test ProConTopologyFactory") {

  // TODO flesh out stub test
  netuit::ProConTopologyFactory{}(100);

  for (size_t i = 0; i < 150; ++i) {
    REQUIRE( netuit::ProConTopologyFactory{}(i).GetSize() == i );
  }

  for (const auto& node : netuit::ProConTopologyFactory{}(100)) {
    REQUIRE( node.GetNumInputs() + node.GetNumOutputs() == 1);
    REQUIRE( node.HasInputs() != node.HasOutputs() );
  }
}
*/
TEST_CASE("Test file output") {
  REQUIRE(test_all_adj(netuit::ProConTopologyFactory{}));

}