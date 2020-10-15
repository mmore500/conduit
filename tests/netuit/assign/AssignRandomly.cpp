#include <mpi.h>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "../../../third-party/Empirical/source/tools/random_utils.h"

#include "netuit/assign/AssignRandomly.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

const uitsl::MpiGuard guard;

// TODO: stub test
TEST_CASE("Test AssignRandomly") {
  emp::Random rand;
  netuit::AssignRandomly<size_t>(1, rand)(0);
}
