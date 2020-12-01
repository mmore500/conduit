#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/vector.hpp"

#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

#include "uit/ducts/proc/impl/backend/RuntimeSizeRdmaBackEnd.hpp"
#include "uit/setup/ImplSpec.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test RuntimeSizeRdmaBackEnd") {

  // TODO flesh out stub test
  uit::RuntimeSizeRdmaBackEnd<uit::ImplSpec<emp::vector<char>>>{ 42 };

}
