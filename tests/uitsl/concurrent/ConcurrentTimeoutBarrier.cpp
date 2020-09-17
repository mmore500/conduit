#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/concurrent/ConcurrentTimeoutBarrier.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/parallel/ThreadIbarrierFactory.hpp"
#include "uitsl/parallel/ThreadTeam.hpp"

const uitsl::MpiGuard guard;

constexpr size_t num_threads{ 2 };

void do_work() {

  static uitsl::ThreadIbarrierFactory factory{ num_threads };

  const uitsl::ConcurrentTimeoutBarrier barrier{ factory.MakeBarrier() };

}


TEST_CASE("Test ConcurrentTimeoutBarrier") {

  uitsl::ThreadTeam team;

  team.Add(do_work);
  team.Add(do_work);

  team.Join();

}
