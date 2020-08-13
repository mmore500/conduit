#include <memory>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "../../MultiprocessReporter.hpp"

#include "distributed/MPIGuard.hpp"
#include "conduit/ImplSpec.hpp"
#include "conduit/Duct.hpp"
#include "conduit/Inlet.hpp"

const uit::MPIGuard guard;

TEST_CASE("Test Inlet") {

  using Spec = uit::ImplSpec<char>;
  uit::Inlet<Spec>{
    std::make_shared<uit::Duct<Spec>>()
  };

}
