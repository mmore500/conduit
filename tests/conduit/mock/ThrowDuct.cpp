#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/debug/MultiprocessReporter.hpp"
#include "uit/mpi/MPIGuard.hpp"

#define MSG_T int

const uit::MPIGuard guard;

TEST_CASE("Test ThrowDuct") {

  // TODO flesh out stub tests
  uit::ThrowDuct<uit::ImplSpec<MSG_T>>{};

}
