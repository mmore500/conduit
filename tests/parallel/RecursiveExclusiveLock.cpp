#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "parallel/RecursiveExclusiveLock.hpp"
#include "parallel/RecursiveMutex.hpp"

TEST_CASE("RecursiveExclusiveLock") {

  // TODO expand stub test
  uit::RecursiveMutex mutex;
  uit::RecursiveExclusiveLock{ mutex };

}
