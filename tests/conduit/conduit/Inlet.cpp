#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/Duct.hpp"
#include "conduit/ImplSpec.hpp"
#include "conduit/Inlet.hpp"
#include "distributed/MPIGuard.hpp"
#include "distributed/MultiprocessReporter.hpp"

const uit::MPIGuard guard;

TEST_CASE("Test Inlet") {

  using Spec = uit::ImplSpec<char>;
  uit::Inlet<Spec>{
    std::make_shared<uit::Duct<Spec>>()
  };

}
