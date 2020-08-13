#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "../MultiprocessReporter.hpp"

#include "distributed/group_utils.hpp"
#include "distributed/mpi_utils.hpp"
#include "distributed/MPIGuard.hpp"

const uit::MPIGuard guard;

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
