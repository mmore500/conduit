#include "mpi.h"

#define CATCH_CONFIG_RUNNER
#include "Catch/single_include/catch2/catch.hpp"

#include "distributed/group_utils.h"
#include "distributed/mpi_utils.h"

TEST_CASE("make_group") {

  if (uit::get_rank()) {
    const MPI_Group dyad{
      uit::make_group({0, uit::get_rank()})
    };
    uit::group_to_comm(dyad);
  } else {

    for (uit::proc_id_t target = 1; target < uit::get_nprocs(); ++target) {
      const MPI_Group dyad{
        uit::make_group({0, target})
      };
      uit::group_to_comm(dyad);
    }

  }

}

int main(int argc, char* argv[]) {

  uit::verify(MPI_Init(&argc, &argv));

  int result = Catch::Session{}.run( argc, argv );

  uit::verify(MPI_Finalize());

  return result;
}
