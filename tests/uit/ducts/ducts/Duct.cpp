#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/ducts/Duct.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test Duct") {

  // TODO flesh out stub test
  uit::internal::Duct<uit::ImplSpec<char>>{};

}
