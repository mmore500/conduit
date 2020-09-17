#include <string>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/containers/safe/unordered_map.hpp"

TEST_CASE("unordered_map") {

  // TODO expand stub test
  uit::safe::unordered_map<std::string, size_t> map;

  REQUIRE(map.count("hello") == 0);

  map["hello"] = 42;

  REQUIRE(map.count("hello") == 1);
  REQUIRE(map.at("hello") == 42);

}
