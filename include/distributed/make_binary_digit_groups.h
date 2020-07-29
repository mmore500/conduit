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

  const int rank = get_rank(comm);
  for (size_t bit = 0; bit < num_groups; ++bit) {

    res.emplace_back();
    verify(MPI_Comm_split(
      comm, // MPI_Comm comm
      test_bit(rank, bit), // int color
      0, // int key
      &res.back() // MPI_Comm * newcomm
    ));

    emp_assert(comm_size(res.back()) >= num_ranks/num_groups );
    emp_assert(comm_size(res.back()) <= num_ranks/num_groups + 1 );

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
