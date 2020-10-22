#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

#include "uit/ducts/proc/accumulating+type=spanfundamental/inlet=Accumulate+outlet=WithdrawingWindow_sf::IaOwwDuct.hpp"
#include "uit/ducts/proc/impl/backend/impl/InletMemoryAccumulatingPool.hpp"
#include "uit/ducts/proc/impl/backend/impl/PoolSpec.hpp"
#include "uit/setup/ImplSpec.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test InletMemoryAccumulatingPool") {

  // TODO flesh out stub test
  using PoolSpec = uit::PoolSpec<
    uit::ImplSpec<char>,
    uit::sf::IaOwwDuct
  >;
  uit::InletMemoryAccumulatingPool<PoolSpec>{};

}
