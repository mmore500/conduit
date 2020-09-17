#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "uit/debug/IsFirstExecutionChecker.hpp"

TEST_CASE("IsFirstExecutionChecker") {

  uit::IsFirstExecutionChecker checker;
  checker.Check();

}
