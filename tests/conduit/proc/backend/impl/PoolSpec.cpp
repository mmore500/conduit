#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/base/vector.h"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/backend/impl/PoolSpec.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping/VectorBlockRingImsgDuct.hpp"
#include "uit/mpi/MpiGuard.hpp"
#include "uit/debug/MultiprocessReporter.hpp"

const uit::MpiGuard guard;

TEST_CASE("Test PoolSpec") {

  // TODO flesh out stub test
  uit::PoolSpec<uit::ImplSpec<char>, uit::VectorBlockRingImsgDuct>{};

}
