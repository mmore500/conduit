#include <algorithm>
#include <set>
#include <thread>

#include <mpi.h>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "../../third-party/Empirical/source/base/vector.h"

#include "uit/concurrent/Gatherer.hpp"
#include "uit/debug/MultiprocessReporter.hpp"
#include "uit/mpi/MpiGuard.hpp"
#include "uit/mpi/mpi_utils.hpp"
#include "uit/utility/CircularIndex.hpp"
#include "uit/debug/safe_cast.hpp"
#include "uit/parallel/ThreadTeam.hpp"
#include "uit/parallel/thread_utils.hpp"
#include "uit/polyfill/barrier.hpp"

const uit::MpiGuard guard;

constexpr size_t num_threads{ 2 };

uit::Gatherer<int> gather(MPI_INT);

void do_work() {

  static std::barrier barrier{uit::safe_cast<std::ptrdiff_t>(num_threads)};

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
