#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/concurrent/ConcurrentTimeoutBarrier.hpp"
#include "uit/distributed/MPIGuard.hpp"
#include "uit/distributed/MultiprocessReporter.hpp"
#include "uit/parallel/ThreadIbarrierFactory.hpp"
#include "uit/parallel/ThreadTeam.hpp"

const uit::MPIGuard guard;

constexpr size_t num_threads{ 2 };

void do_work() {

  static uit::ThreadIbarrierFactory factory{ num_threads };

  const uit::ConcurrentTimeoutBarrier barrier{ factory.MakeBarrier() };

}


TEST_CASE("Test ConcurrentTimeoutBarrier") {

  uit::ThreadTeam team;

  team.Add(do_work);
  team.Add(do_work);

  team.Join();

}
