#include <string>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "containers/safe/deque.hpp"

TEST_CASE("deque") {

  // TODO expand stub test
  uit::safe::deque<std::string> deque;

  REQUIRE(deque.empty());
  deque.push_back("hello");
  REQUIRE(deque[0] == "hello");

  REQUIRE(deque.back() == "hello");

}
