#include <thread>
#include <set>
#include <algorithm>

#include <mpi.h>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "distributed/MultiprocessReporter.hpp"

#include "../../third-party/Empirical/source/base/vector.h"

#include "distributed/mpi_utils.hpp"
#include "distributed/MPIGuard.hpp"
#include "concurrent/Gatherer.hpp"
#include "utility/CircularIndex.hpp"
#include "parallel/ThreadTeam.hpp"
#include "polyfill/barrier.hpp"

#include "distributed/mpi_utils.hpp"
#include "parallel/thread_utils.hpp"
#include "utility/numeric_cast.hpp"

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
