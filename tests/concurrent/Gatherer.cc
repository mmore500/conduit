#include <thread>
#include <set>
#include <algorithm>

#include <mpi.h>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "../MultiprocessReporter.h"

#include "../../third-party/Empirical/source/base/vector.h"

#include "distributed/mpi_utils.h"
#include "distributed/MPIGuard.h"
#include "concurrent/Gatherer.h"
#include "utility/CircularIndex.h"
#include "parallel/ThreadTeam.h"
#include "polyfill/barrier.h"

#include "distributed/mpi_utils.h"
#include "parallel/thread_utils.h"
#include "utility/numeric_cast.h"

const uit::MPIGuard guard;

constexpr size_t num_threads{ 2 };

uit::Gatherer<int> gather(MPI_INT);

void do_work() {

  static std::barrier barrier{uit::numeric_cast<std::ptrdiff_t>(num_threads)};

  barrier.arrive_and_wait();

  gather.Put(uit::get_thread_id() + num_threads * uit::get_proc_id());

}


TEST_CASE("Test Gatherer") {

  uit::ThreadTeam team;

  team.Add(do_work);
  team.Add(do_work);

  team.Join();

  const auto res = gather.Gather();

  if (uit::is_root()) REQUIRE( res );

  if (res) {
    emp::vector<int> target(num_threads * uit::get_nprocs());
    std::iota(std::begin(target), std::end(target), 0);

    REQUIRE(
      std::set(std::begin(*res), std::end(*res))
        == std::set(std::begin(target), std::end(target))
    );

  };

}
