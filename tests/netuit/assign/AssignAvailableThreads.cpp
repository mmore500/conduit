#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/assign/AssignAvailableThreads.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

// TODO: stub test
TEST_CASE("Test AssignAvailableThreads") {
  netuit::AssignAvailableThreads{}(1);
}
