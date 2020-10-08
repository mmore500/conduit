#include <fstream>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/topology/ToroidalTopologyFactory.hpp"
#include "factory_common.hpp"

/*
TEST_CASE("Test ToroidalTopologyFactory") {

  // N-dimensional case

  for (size_t i = 0; i < 150; ++i) {
    REQUIRE( uit::ToroidalTopologyFactory{}({i, i}).GetSize() == i * i );
  }

  for (const auto& node : uit::ToroidalTopologyFactory{}({10, 10})) {
    REQUIRE( node.GetNumInputs() == 4);
    REQUIRE( node.GetNumOutputs() == 4);
  }
}
*/
TEST_CASE("Test file output") {
  REQUIRE(test_all_adj(uit::ToroidalTopologyFactory{}));

/*
  std::ofstream file1;
  file1.open("1-torus-color.adj");
  auto topo2 = uit::ToroidalTopologyFactory{}({100});
  auto res2 = topo2.Optimize(5);
  topo2.PrintPartition(res2, file1);
  file1.close();

  std::ofstream file2;
  file2.open("2-torus-color.adj");

  auto topo = uit::ToroidalTopologyFactory{}({10, 10});

  topo.PrintNodes();
  topo.AsCSR();

  auto res = topo.Optimize(5);

  topo.PrintPartition(res, file2);
  file2.close();



  std::ofstream file3;
  file3.open("3-torus.adj");
  uit::ToroidalTopologyFactory{}({4, 4, 4}).PrintNodes(file3);
  file3.close();
*/
}