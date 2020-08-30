#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/distributed/IsendFunctor.hpp"
#include "uit/distributed/MPIGuard.hpp"
#include "uit/distributed/MultiprocessReporter.hpp"

const uit::MPIGuard guard;

TEST_CASE("Test IsendFunctor") {

  // TODO flesh out stub test
  uit::IsendFunctor{};

}
