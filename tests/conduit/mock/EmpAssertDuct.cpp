#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/EmpAssertDuct.hpp"
#include "uit/debug/MultiprocessReporter.hpp"
#include "uit/mpi/MpiGuard.hpp"

#define MSG_T int

const uit::MpiGuard guard;

TEST_CASE("Test EmpAssertDuct") {

  // TODO flesh out stub tests
  uit::EmpAssertDuct<uit::ImplSpec<MSG_T>>{};

}
