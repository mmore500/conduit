#include <unordered_set>

#include <mpi.h>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/mpi/MpiGuard.hpp"
#include "uit/mpi/mpi_utils.hpp"
#include "uit/debug/MultiprocessReporter.hpp"
#include "uit/mpi/Request.hpp"
#include "uit/utility/assign_utils.hpp"
#include "uit/utility/math_utils.hpp"

const uit::MpiGuard guard;

TEST_CASE("test_null") {

  uit::Request req;

  REQUIRE( uit::test_null(req) );

  char buf;
  UIT_Irecv(
    &buf, // void *buf
    1, // int count
    MPI_BYTE, // MPI_Datatype datatype
    MPI_ANY_SOURCE, // int source
    MPI_ANY_TAG, // int tag
    MPI_COMM_WORLD, // MPI_Comm comm
    &req // MPI_Request *request
  );

  REQUIRE( !uit::test_null(req) );

  UIT_Cancel(&req);
  UIT_Request_free(&req);

  REQUIRE( uit::test_null(req) );

}
