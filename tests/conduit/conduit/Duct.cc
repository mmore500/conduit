#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "../../MultiprocessReporter.h"

#include "distributed/MPIGuard.h"
#include "conduit/Duct.h"
#include "conduit/ImplSpec.h"

const uit::MPIGuard guard;

TEST_CASE("Test Duct") {

  // TODO flesh out stub test
  uit::Duct<uit::ImplSpec<char>>{};

}
