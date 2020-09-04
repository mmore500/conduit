#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/mpi/MpiGuard.hpp"
#include "uit/mpi/Request.hpp"
#include "uit/debug/MultiprocessReporter.hpp"

const uit::MpiGuard guard;

TEST_CASE("Test Request") {

  uit::Request request;
  REQUIRE( request == MPI_REQUEST_NULL );

}
