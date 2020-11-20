#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/assign/GenerateMetisAssignments.hpp"
#include "netuit/topology/Topology.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

// TODO: stub test
TEST_CASE("Test GenerateMetisAssignments") {
  netuit::Topology topo;
  netuit::GenerateMetisAssignments(1, 1, topo);
}
