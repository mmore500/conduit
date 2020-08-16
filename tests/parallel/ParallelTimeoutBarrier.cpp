#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "parallel/ThreadIbarrierFactory.hpp"
#include "parallel/ParallelTimeoutBarrier.hpp"

TEST_CASE("ParallelTimeoutBarrier") {

  // TODO expand stub test
  const size_t num_threads{ 1 };
  static uit::ThreadIbarrierFactory factory{ num_threads };
  uit::ParallelTimeoutBarrier{ factory.MakeBarrier() };

}
