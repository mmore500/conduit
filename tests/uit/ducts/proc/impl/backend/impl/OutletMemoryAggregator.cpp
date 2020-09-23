#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

#include "uit/ducts/proc/impl/backend/impl/AggregatorSpec.hpp"
#include "uit/ducts/proc/impl/backend/impl/OutletMemoryAggregator.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=cereal/inlet=RingIsend+outlet=Iprobe_c::IriOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test OutletMemoryAggregator") {

  // TODO flesh out stub test
  using AggregatorSpec = uit::AggregatorSpec<
    uit::ImplSpec<char>,
    uit::c::IriOiDuct
  >;
  uit::OutletMemoryAggregator<AggregatorSpec>{};

}
