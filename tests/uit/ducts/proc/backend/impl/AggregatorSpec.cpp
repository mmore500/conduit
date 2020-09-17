#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/base/vector.h"

#include "uit/setup/ImplSpec.hpp"
#include "uit/ducts/proc/backend/impl/AggregatorSpec.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=cereal/inlet=RingIsend+outlet=Iprobe_c::IriOiDuct.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test AggregatorSpec") {

  // TODO flesh out stub test
  uit::AggregatorSpec<uit::ImplSpec<char>, uit::c::IriOiDuct>{};

}
