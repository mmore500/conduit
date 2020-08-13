#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "../../MultiprocessReporter.hpp"

#include "distributed/MPIGuard.hpp"
#include "conduit/Conduit.hpp"
#include "conduit/ImplSpec.hpp"

const uit::MPIGuard guard;

TEST_CASE("Test Conduit") {

  // TODO flesh out stub test
  uit::Conduit<uit::ImplSpec<char>> conduit;
  conduit.get<0>();
  conduit.get<1>();

  auto& [inlet, outlet] = conduit;

}
