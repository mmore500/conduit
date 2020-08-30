#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/Duct.hpp"
#include "uit/conduit/ImplSpec.hpp"
#include "uit/distributed/MultiprocessReporter.hpp"
#include "uit/distributed/MPIGuard.hpp"

const uit::MPIGuard guard;

TEST_CASE("Test Duct") {

  // TODO flesh out stub test
  uit::internal::Duct<uit::ImplSpec<char>>{};

}
