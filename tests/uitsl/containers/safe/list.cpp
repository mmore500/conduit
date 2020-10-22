#include <string>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/containers/safe/list.hpp"

TEST_CASE("test list") {

  // TODO expand stub test
  uitsl::safe::list<std::string> list;

  REQUIRE(list.empty());
  list.push_back("hello");
  REQUIRE(list.front() == "hello");

  REQUIRE(list.back() == "hello");

}
