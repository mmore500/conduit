#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "parallel/OncePerThreadChecker.hpp"

// TODO write multithreaded test

TEST_CASE("ThreadIbarrier satisfied") {

  uit::ThreadIbarrierFactory factory{1};
  uit::ThreadIbarrier first{ factory.MakeBarrier() };
  uit::ThreadIbarrier second{ factory.MakeBarrier() };
  REQUIRE(first.IsComplete());
  REQUIRE(second.IsComplete());

}

TEST_CASE("ThreadIbarrier unsatisfied") {

  // TODO expand stub test
  uit::ThreadIbarrierFactory factory{2};
  uit::ThreadIbarrier first{ factory.MakeBarrier() };
  uit::ThreadIbarrier second{ factory.MakeBarrier() };
  REQUIRE(!first.IsComplete());
  REQUIRE(!second.IsComplete());

}
