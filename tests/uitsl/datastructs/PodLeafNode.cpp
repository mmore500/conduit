#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/datastructs/PodLeafNode.hpp"

TEST_CASE("Test PodLeafNode") {

  // TODO expand stub test
  uitsl::PodLeafNode<char, 2>{};

  uitsl::PodLeafNode<char, 2>{}.Reset();

}
