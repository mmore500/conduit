#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/IsFirstExecutionChecker.hpp"

TEST_CASE("IsFirstExecutionChecker") {

  uitsl::IsFirstExecutionChecker checker;
  checker.Check();

}
