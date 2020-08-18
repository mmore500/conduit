#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "utility/IsFirstExecutionChecker.hpp"

TEST_CASE("IsFirstExecutionChecker") {

  uit::IsFirstExecutionChecker checker;
  checker.Check();

}
