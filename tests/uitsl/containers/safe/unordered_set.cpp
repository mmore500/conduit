#include <string>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/containers/safe/unordered_set.hpp"

TEST_CASE("unordered_set", "[nproc:1]") {

  // TODO expand stub test
  uitsl::safe::unordered_set<std::string> map;

  REQUIRE(map.count("hello") == 0);

  map.insert("hello");

  REQUIRE(map.count("hello") == 1);

}
