#include <string>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/containers/safe/vector.hpp"

TEST_CASE("vector") {

  // TODO expand stub test
  uit::safe::vector<std::string> vector;

  REQUIRE(vector.empty());
  vector.push_back("hello");
  REQUIRE(vector[0] == "hello");

  REQUIRE(vector.back() == "hello");

}
