#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/ImplSpec.hpp"
#include "conduit/mock/EmpAssertDuct.hpp"
#include "distributed/MultiprocessReporter.hpp"
#include "distributed/MPIGuard.hpp"

#define MSG_T int

const uit::MPIGuard guard;

TEST_CASE("Test EmpAssertDuct") {

  // TODO flesh out stub tests
  uit::EmpAssertDuct<uit::ImplSpec<MSG_T>>{};

}
