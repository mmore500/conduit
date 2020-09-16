#pragma once

#include <set>
#include <sstream>

#include <mpi.h>

#include "../../../third-party/Empirical/source/base/assert.h"

#include "../utility/print_utils.hpp"

#include "audited_routines.hpp"
#include "group_utils.hpp"
#include "proc_id_t.hpp"

namespace uit {

int get_nprocs(const MPI_Comm& comm=MPI_COMM_WORLD) {
  int res;
  UIT_Comm_size(
    comm,
    &res
  );
  return res;
}

//TODO replace with get_proc_id
proc_id_t get_rank(const MPI_Comm& comm=MPI_COMM_WORLD) {
  int res;
  UIT_Comm_rank(
    comm,
    &res
  );
  return res;
}

// TODO rename get_pid
proc_id_t get_proc_id(const MPI_Comm& comm=MPI_COMM_WORLD) {
  return get_rank(comm);
}

MPI_Group comm_to_group(const MPI_Comm & comm){
  MPI_Group group;
  UIT_Comm_group(
    comm, // MPI_Comm comm
    &group // MPI_Group* group
  );
  return group;
}

std::string get_name(const MPI_Comm& comm) {
  int len;
  emp::array<char, MPI_MAX_OBJECT_NAME> buffer;
  UIT_Comm_get_name(comm, buffer.data(), &len);
  return std::string{}.assign(buffer.data(), len);
}

MPI_Comm duplicate_comm(const MPI_Comm& comm) {
  MPI_Comm res;
  UIT_Comm_dup(
    comm,
    &res
  );
  return res;
}

size_t comm_size(const MPI_Comm& comm) {
  const int res{ get_nprocs(comm) };
  emp_assert(res >= 0);
  return res;
}

MPI_Comm split_comm(
  const std::function<int(const int)> colorer,
  const MPI_Comm& comm=MPI_COMM_WORLD
) {

  MPI_Comm res;
  MPI_Comm_split(
    comm, // MPI_Comm comm
    colorer(get_rank(comm)), // int color
    0, // int key
    &res // MPI_Comm * newcomm
  );
  return res;

}

emp::vector<proc_id_t> get_comm_ranks(const MPI_Comm& comm) {
  return uit::get_group_ranks(uit::comm_to_group(comm));
}

proc_id_t translate_comm_rank(
  const proc_id_t rank,
  const MPI_Comm& from,
  const MPI_Comm& to=MPI_COMM_WORLD
) {
  return uit::translate_group_rank(
    rank, uit::comm_to_group(from), uit::comm_to_group(to)
  );
}

bool is_multiprocess(const MPI_Comm& comm=MPI_COMM_WORLD) {
  return uit::get_nprocs(comm) > 1;
}

bool is_root(const MPI_Comm& comm=MPI_COMM_WORLD) {
  return uit::get_rank(comm) == 0;
}

std::string comm_to_string(const MPI_Comm& comm) {
  std::stringstream ss;
  ss << uit::format_member(
    "uit::comm_size(comm)", uit::comm_size(comm)
  ) << std::endl;
  ss << uit::format_member(
    "uit::get_comm_ranks(comm)", uit::to_string(uit::get_comm_ranks(comm))
  ) << std::endl;
  ss << uit::format_member(
    "uit::get_name(comm)", uit::get_name(comm)
  ) << std::endl;
  return ss.str();
}

} // namespace uit
