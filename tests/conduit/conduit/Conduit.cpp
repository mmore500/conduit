#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/Conduit.hpp"
#include "uit/conduit/ImplSpec.hpp"
#include "uit/distributed/MultiprocessReporter.hpp"
#include "uit/distributed/MPIGuard.hpp"

const uit::MPIGuard guard;

TEST_CASE("Test Conduit") {

  // TODO flesh out stub test
  uit::Conduit<uit::ImplSpec<char>> conduit;
  conduit.get<0>();
  conduit.get<1>();

  [[maybe_unused]] auto& [inlet, outlet] = conduit;

}
