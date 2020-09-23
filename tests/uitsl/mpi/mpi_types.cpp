#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/mpi_types.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("byte") { REQUIRE(uitsl::datatype_from_type<std::byte>() == MPI_BYTE); }

TEST_CASE("int") { REQUIRE(uitsl::datatype_from_type<int>() == MPI_INT); }

TEST_CASE("double") {
  REQUIRE(uitsl::datatype_from_type<double>() == MPI_DOUBLE);
}
