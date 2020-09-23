#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/concurrent/ThreadSafeIbarrierRequest.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/parallel/ThreadTeam.hpp"

const uitsl::MpiGuard guard;

constexpr size_t num_threads{ 2 };

void do_work() {

  const uitsl::ThreadSafeIbarrierRequest barrier;
  while( !barrier.IsComplete() );
  REQUIRE( barrier.IsComplete() );

}


TEST_CASE("Test ThreadSafeIbarrierRequest") {

  uitsl::ThreadTeam team;

  team.Add(do_work);
  team.Add(do_work);

  team.Join();

}
