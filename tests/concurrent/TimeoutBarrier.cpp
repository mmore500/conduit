#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "concurrent/TimeoutBarrier.hpp"
#include "distributed/MPIGuard.hpp"
#include "distributed/MultiprocessReporter.hpp"
#include "parallel/ThreadIbarrierFactory.hpp"
#include "parallel/ThreadTeam.hpp"

const uit::MPIGuard guard;

constexpr size_t num_threads{ 2 };

void do_work() {

  static uit::ThreadIbarrierFactory factory{ num_threads };

  const uit::TimeoutBarrier barrier{ factory.MakeBarrier() };

}


TEST_CASE("Test TimeoutBarrier") {

  uit::ThreadTeam team;

  team.Add(do_work);
  team.Add(do_work);

  team.Join();

}
