#include <mpi.h>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "assign/GenerateMetisAssignments.hpp"
#include "distributed/MultiprocessReporter.hpp"
#include "distributed/MPIGuard.hpp"

const uit::MPIGuard guard;

// stub test
