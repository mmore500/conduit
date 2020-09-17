#include <set>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/InterProcAddress.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"

const uit::MpiGuard guard;

TEST_CASE("Test InterProcAddress") {

  // TODO flesh out stub test
  uit::InterProcAddress{0};

  REQUIRE( uit::InterProcAddress{0} == uit::InterProcAddress{0} );
  REQUIRE(!( uit::InterProcAddress{0} < uit::InterProcAddress{0} ));
  REQUIRE( uit::InterProcAddress{0} != uit::InterProcAddress{1} );

  std::set<uit::InterProcAddress>{};

}
