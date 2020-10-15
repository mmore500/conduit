#include <mpi.h>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/assign/AssignIntegrated.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

const uitsl::MpiGuard guard;

// TODO: stub test
TEST_CASE("Test AssignIntegrated") {
  netuit::AssignIntegrated<size_t> assign;
}
