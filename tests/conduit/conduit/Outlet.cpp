#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/ImplSpec.hpp"
#include "conduit/Outlet.hpp"
#include "distributed/MultiprocessReporter.hpp"
#include "distributed/MPIGuard.hpp"

const uit::MPIGuard guard;

TEST_CASE("Test Outlet") {

  using Spec = uit::ImplSpec<char>;
  uit::Outlet<Spec>{
    std::make_shared<uit::Duct<Spec>>()
  };

}
