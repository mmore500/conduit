#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

#include "uit/setup/ImplSpec.hpp"
#include "uit/spouts/Outlet.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test Outlet") {

  using Spec = uit::ImplSpec<char>;
  uit::Outlet<Spec>{
    std::make_shared<uit::internal::Duct<Spec>>()
  };


  SECTION("Test impl detectors") {

    using Spec = uit::ImplSpec<char>;
    uit::Outlet<Spec> out{
      std::make_shared<uit::internal::Duct<Spec>>()
    };

    REQUIRE( out.HoldsIntraImpl().value_or(false) == true );
    REQUIRE( out.HoldsThreadImpl().value_or(false) == false );
    REQUIRE( out.HoldsProcImpl().value_or(false) == false );

  }
}
