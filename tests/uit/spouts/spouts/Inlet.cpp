#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

#include "uit/ducts/Duct.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/spouts/Inlet.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test Inlet") {

  using Spec = uit::ImplSpec<char>;
  uit::Inlet<Spec>{
    std::make_shared<uit::internal::Duct<Spec>>()
  };

}

TEST_CASE("Test impl detectors") {

  using Spec = uit::ImplSpec<char>;
  uit::Inlet<Spec> in{
    std::make_shared<uit::internal::Duct<Spec>>()
  };

  REQUIRE( in.HoldsIntraImpl().value_or(false) == true );
  REQUIRE( in.HoldsThreadImpl().value_or(false) == false );
  REQUIRE( in.HoldsProcImpl().value_or(false) == false );

}
