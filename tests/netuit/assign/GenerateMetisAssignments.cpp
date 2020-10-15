#include <mpi.h>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/assign/GenerateMetisAssignments.hpp"
#include "netuit/topology/Topology.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

const uitsl::MpiGuard guard;

// TODO: stub test
TEST_CASE("Test GenerateMetisAssignments") {
  netuit::Topology topo;
  netuit::GenerateMetisAssignments(1, 1, topo);
}
