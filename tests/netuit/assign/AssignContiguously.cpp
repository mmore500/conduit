#include <mpi.h>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/assign/AssignContiguously.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

const uitsl::MpiGuard guard;

// TODO: stub test
TEST_CASE("Test AssignContiguously") {
  netuit::AssignContiguously<size_t>(1, 1)(1);
}
