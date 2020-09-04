#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/backend/MockBackEnd.hpp"
#include "uit/mpi/MPIGuard.hpp"
#include "uit/debug/MultiprocessReporter.hpp"

const uit::MPIGuard guard;

TEST_CASE("Test MockBackEnd") {

  // TODO flesh out stub test
  uit::MockBackEnd<uit::ImplSpec<char>>{};

}
