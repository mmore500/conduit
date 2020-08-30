#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/parallel/RecursiveExclusiveLock.hpp"
#include "uit/parallel/RecursiveMutex.hpp"

TEST_CASE("RecursiveExclusiveLock") {

  // TODO expand stub test
  uit::RecursiveMutex mutex;
  uit::RecursiveExclusiveLock{ mutex };

}
