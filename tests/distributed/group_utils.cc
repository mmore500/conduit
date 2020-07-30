#include "mpi.h"

#define CATCH_CONFIG_RUNNER
#include "Catch/single_include/catch2/catch.hpp"

#include "distributed/group_utils.h"
#include "distributed/mpi_utils.h"

TEST_CASE("make_group") {

  if (get_rank()) {
    const MPI_Group dyad{
      make_group({0, get_rank()})
    };
    group_to_comm(dyad);
  } else {

    for (proc_id_t target = 1; target < get_nprocs(); ++target) {
      const MPI_Group dyad{
        make_group({0, target})
      };
      group_to_comm(dyad);
    }

  }

}

int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  int result = Catch::Session{}.run( argc, argv );

  MPI_Finalize();

  return result;
}
