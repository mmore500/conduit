#pragma once

#include "mpi.h"

#include "tools/math.h"

#include "make_binary_digit_groups.h"

MPI_Group make_singleton_group(
  const int rank,
  MPI_Comm comm=MPI_COMM_WORLD
) {

  // TODO refactor to make this a class?
  emp::vector<MPI_Group> binary_groups = make_binary_digit_groups(comm);

  const int this_rank = get_rank(comm);

  emp::vector<MPI_Group> includes;
  emp::vector<MPI_Group> excludes;
  for (size_t bit = 0; bit < binary_groups.size(); ++bit) {
    if (test_bit(rank, bit) == test_bit(this_rank, bit)) {
      includes.push_back(binary_groups[bit]);
    } else {
      excludes.push_back(binary_groups[bit]);
    }
  }

  const MPI_Group include = intersect_groups(includes);
  const MPI_Group exclude = combine_groups(excludes);

  emp_assert(include != MPI_GROUP_NULL);
  emp_assert(exclude != MPI_GROUP_NULL);
  emp_assert(include != MPI_GROUP_EMPTY);

  const MPI_Group res = subtract_groups(include, exclude);
  emp_assert(group_size(res) == 1);

  return res;

}
