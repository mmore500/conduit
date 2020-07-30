#pragma once

#include "mpi.h"

#include "tools/math.h"

#include "utility/math_utils.h"
#include "utility/safe_compare.h"

#include "mpi_utils.h"

emp::vector<MPI_Comm> make_binary_digit_comms(MPI_Comm comm=MPI_COMM_WORLD) {

  // TODO add a way to free these comms
  static emp::vector<MPI_Comm> res;

  const size_t num_ranks = comm_size(comm);
  const size_t num_groups = num_bits(num_ranks);

  if (res.size() == num_groups) {
    return res;
  }

  emp_assert(res.size() == 0);

  const proc_id_t rank = get_rank(comm);
  for (size_t bit = 0; bit < num_groups; ++bit) {

    res.emplace_back();
    verify(MPI_Comm_split(
      comm, // MPI_Comm comm
      test_bit(rank, bit), // int color
      0, // int key
      &res.back() // MPI_Comm * newcomm
    ));

    emp_assert(
      comm_size(res.back()) >= num_ranks/num_groups,
      bit,
      comm_size(res.back())
    );
    emp_assert(
      comm_size(res.back()) <= num_ranks/num_groups + 1,
      bit,
      comm_size(res.back())
    );

  }

  return res;

}

emp::vector<MPI_Group> make_binary_digit_groups(MPI_Comm comm=MPI_COMM_WORLD) {

  emp::vector<MPI_Comm> comms{ make_binary_digit_comms(comm) };

  emp::vector<MPI_Group> res;
  std::transform(
    std::begin(comms),
    std::end(comms),
    std::back_inserter(res),
    comm_to_group
  );

  emp_assert(safe_equal(res.size(), num_bits(get_nprocs(comm))));

  return res;

}

MPI_Group make_singleton_group(
  const proc_id_t rank,
  MPI_Comm comm=MPI_COMM_WORLD
) {

  // TODO refactor to make this a class?
  emp::vector<MPI_Group> binary_groups = make_binary_digit_groups(comm);

  const proc_id_t this_rank = get_rank(comm);

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

MPI_Group make_dyad_group(
  const proc_id_t rank1,
  const proc_id_t rank2
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
