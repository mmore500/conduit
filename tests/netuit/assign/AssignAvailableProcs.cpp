#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/assign/AssignAvailableProcs.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

// TODO: stub test
TEST_CASE("Test AssignAvailableProcs") {
  netuit::AssignAvailableProcs{}(1);
}
