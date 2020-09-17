#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/base/vector.h"

#include "uit/config/ImplSpec.hpp"
#include "uit/duct/proc/backend/impl/PoolSpec.hpp"
#include "uit/duct/proc/put=dropping+get=skipping+type=span/inlet=RingIsend+outlet=BlockIrecv_s::IriObiDuct.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test PoolSpec") {

  // TODO flesh out stub test
  uit::PoolSpec<uit::ImplSpec<char>, uit::s::IriObiDuct>{};

}
