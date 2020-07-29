#pragma once

#include "mpi.h"

#include "mpi_utils.h"

#include "make_singleton_group.h"

// should depend on make_singleton_groups
MPI_Group make_dyad_group(
  const int rank1,
  const int rank2
) {

  MPI_Group res;

  // TODO free these singleton groups?
  verify(MPI_Group_union(
  	make_singleton_group(rank1), // MPI_Group group1
  	make_singleton_group(rank2), // MPI_Group group2
    &res
	));

  return res;

}
