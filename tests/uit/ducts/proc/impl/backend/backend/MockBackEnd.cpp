#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

#include "uit/ducts/proc/impl/backend/MockBackEnd.hpp"
#include "uit/setup/ImplSpec.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test MockBackEnd") {

  // TODO flesh out stub test
  uit::MockBackEnd<uit::ImplSpec<char>>{};

}
