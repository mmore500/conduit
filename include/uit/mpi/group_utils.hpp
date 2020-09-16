#pragma once

#include <set>
#include <sstream>

#include <mpi.h>

#include "../../../third-party/Empirical/source/base/assert.h"
#include "../../../third-party/Empirical/source/tools/math.h"

#include "../utility/math_utils.hpp"
#include "../utility/print_utils.hpp"
#include "../debug/safe_compare.hpp"

#include "audited_routines.hpp"
#include "proc_id_t.hpp"

namespace uit {

// predeclaration
MPI_Group comm_to_group(const MPI_Comm &);

MPI_Group intersect_groups(emp::vector<MPI_Group> groups) {

  MPI_Group res{
    groups.size() ? groups.back() : MPI_GROUP_EMPTY
  };

  // TODO more MPI_Group_free ?
  for ( ; groups.size(); groups.pop_back()) {

    MPI_Group temp;
    UIT_Group_intersection(
      res, // MPI_Group group1,
      groups.back(), // MPI_Group group2,
      &temp // MPI_Group* newgroup
    );
    UIT_Group_free(
      &res // MPI_Group* group
    );
    res = temp;
  }

  return res;

}

MPI_Group combine_groups(emp::vector<MPI_Group> groups) {

  MPI_Group res{
    groups.size() ? groups.back() : MPI_GROUP_EMPTY
  };

  // TODO more MPI_Group_free ?
  for ( ; groups.size(); groups.pop_back()) {

    MPI_Group temp;
    UIT_Group_union(
      res, // MPI_Group group1
      groups.back(), // MPI_Group group2
      &temp // MPI_Group* newgroup
    );
    UIT_Group_free(
      &res // MPI_Group* group
    );
    res = temp;
  }

  return res;

}

MPI_Group subtract_groups(
  const MPI_Group & minuend,
  const MPI_Group & subtrahend
) {

  MPI_Group res;

  UIT_Group_difference(
    minuend, // MPI_Group group1
    subtrahend, // MPI_Group group2
    &res // MPI_Group * newgroup
  );

  return res;

}

size_t group_size(const MPI_Group & group) {
  int res;
  UIT_Group_size(
    group, // MPI_Group group
    &res // int *size
  );
  emp_assert(res >= 0);
  return res;
}

MPI_Comm group_to_comm(
  const MPI_Group & group,
  const MPI_Comm & comm=MPI_COMM_WORLD
) {
  MPI_Comm res;
  UIT_Comm_create_group(
    comm, // MPI_Comm comm
    group, // MPI_Group group
    0, // int tag TODO is this important?
    &res // MPI_Comm * newcomm
  );
  return res;
}

MPI_Group make_group(
  emp::vector<proc_id_t> ranks,
  const MPI_Group source=uit::comm_to_group(MPI_COMM_WORLD)
) {

  std::sort(std::begin(ranks), std::end(ranks));
  const auto last{ std::unique(std::begin(ranks), std::end(ranks)) };
  ranks.erase(last, std::end(ranks));

  emp_assert(std::set<proc_id_t>(
    std::begin(ranks),
    std::end(ranks)
  ).size() == ranks.size(), uit::to_string(ranks));
  emp_assert(std::all_of(
    std::begin(ranks),
    std::end(ranks),
    [&](const auto & rank){
      return uit::safe_less(rank, uit::group_size(source)) && rank >= 0;
    }
  ), uit::to_string(ranks));

  MPI_Group res;
  UIT_Group_incl(
    source, // MPI_Group group
    ranks.size(), // int n
    ranks.data(), // const int ranks[]
    &res // MPI_Group * newgroup
  );
  return res;
}

proc_id_t translate_group_rank(
  const proc_id_t rank,
  const MPI_Group& from,
  const MPI_Group& to=uit::comm_to_group(MPI_COMM_WORLD)
) {
  proc_id_t res;
  UIT_Group_translate_ranks(
    from, // MPI_Group group1
    1, // int n
    &rank, // const int ranks1[]
    to, // MPI_Group group2
    &res // int ranks2[]
  );
  return res;
}

emp::vector<proc_id_t> get_group_ranks(const MPI_Group& group) {

  emp::vector<proc_id_t> within_group_ranks(group_size(group));
  std::iota(
    std::begin(within_group_ranks),
    std::end(within_group_ranks),
    0
  );

  emp::vector<proc_id_t> within_world_ranks(within_group_ranks.size());
  UIT_Group_translate_ranks(
    group, // MPI_Group group1
    within_group_ranks.size(), // int n
    within_group_ranks.data(), // const int ranks1[]
    comm_to_group(MPI_COMM_WORLD), // MPI_Group group2
    within_world_ranks.data() // int ranks2[]
  );

  return within_world_ranks;

}

std::string group_to_string(const MPI_Group& group) {
  std::stringstream ss;
  ss << format_member(
    "uit::group_size(group)", uit::group_size(group)
  ) << std::endl;
  ss << format_member(
    "uit::get_group_ranks(group)", uit::to_string(uit::get_group_ranks(group))
  ) << std::endl;
  return ss.str();
}

} // namespace uit
