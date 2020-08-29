#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/ImplSpec.hpp"
#include "conduit/proc/backend/MockBackEnd.hpp"
#include "distributed/MPIGuard.hpp"
#include "distributed/MultiprocessReporter.hpp"

const uit::MPIGuard guard;

TEST_CASE("Test MockBackEnd") {

  // TODO flesh out stub test
  uit::internal::MockBackEnd<uit::ImplSpec<char>>{};

}
