#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/NopDuct.hpp"
#include "uit/distributed/MultiprocessReporter.hpp"
#include "uit/distributed/MPIGuard.hpp"

#define MSG_T int

const uit::MPIGuard guard;

TEST_CASE("Test NopDuct") {

  // TODO flesh out stub tests
  uit::NopDuct<uit::ImplSpec<MSG_T>>{};

}
