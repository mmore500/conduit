#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/mpi/mpi_types.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"

const uit::MpiGuard guard;

TEST_CASE("byte") { REQUIRE(uit::datatype_from_type<std::byte>() == MPI_BYTE); }

TEST_CASE("int") { REQUIRE(uit::datatype_from_type<int>() == MPI_INT); }

TEST_CASE("double") {
  REQUIRE(uit::datatype_from_type<double>() == MPI_DOUBLE);
}
