#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/base/vector.h"

#include "uit/setup/ImplSpec.hpp"
#include "uit/ducts/proc/backend/RuntimeSizeBackEnd.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test RuntimeSizeBackEnd") {

  // TODO flesh out stub test
  uit::RuntimeSizeBackEnd<uit::ImplSpec<emp::vector<char>>>{ 42 };

}
