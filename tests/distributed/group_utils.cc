#include "mpi.h"

#define CATCH_CONFIG_RUNNER
#include "Catch/single_include/catch2/catch.hpp"

#include "distributed/group_utils.h"


TEST_CASE("make_binary_digit_comms") {

  // TODO
  const emp::vector<MPI_Comm> bd_comms (
    make_binary_digit_comms()
  );

}

TEST_CASE("make_binary_digit_groups") {

  // TODO
  const emp::vector<MPI_Group> bd_groups (
    make_binary_digit_groups()
  );

}

TEST_CASE("make_singleton_group") {

  // TODO
  const MPI_Group my_singleton{
    make_singleton_group(get_rank())
  };

  const MPI_Group root_singleton{
    make_singleton_group(0)
  };

}

TEST_CASE("make_dyad_group") {

  const MPI_Group dyad{
    make_dyad_group(0, get_rank())
  };

}

int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  int result = Catch::Session{}.run( argc, argv );

  MPI_Finalize();

  return result;
}
