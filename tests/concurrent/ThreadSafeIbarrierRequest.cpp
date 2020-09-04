#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/concurrent/ThreadSafeIbarrierRequest.hpp"
#include "uit/mpi/MPIGuard.hpp"
#include "uit/debug/MultiprocessReporter.hpp"
#include "uit/parallel/ThreadTeam.hpp"

const uit::MPIGuard guard;

constexpr size_t num_threads{ 2 };

void do_work() {

  const uit::ThreadSafeIbarrierRequest barrier;
  while( !barrier.IsComplete() );
  REQUIRE( barrier.IsComplete() );

}


TEST_CASE("Test ThreadSafeIbarrierRequest") {

  uit::ThreadTeam team;

  team.Add(do_work);
  team.Add(do_work);

  team.Join();

}
