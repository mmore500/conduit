#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/setup/ImplSpec.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

#define MSG_T int

const uitsl::MpiGuard guard;

TEST_CASE("Test ThrowDuct") {

  // TODO flesh out stub tests
  uit::ThrowDuct<uit::ImplSpec<MSG_T>>{};

}
