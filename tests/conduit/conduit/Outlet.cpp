#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/Outlet.hpp"
#include "uit/distributed/MultiprocessReporter.hpp"
#include "uit/distributed/MPIGuard.hpp"

const uit::MPIGuard guard;

TEST_CASE("Test Outlet") {

  using Spec = uit::ImplSpec<char>;
  uit::Outlet<Spec>{
    std::make_shared<uit::internal::Duct<Spec>>()
  };

}
