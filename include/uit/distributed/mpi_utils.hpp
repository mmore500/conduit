#pragma once

#include <assert.h>
#include <limits>
#include <numeric>
#include <stddef.h>

#include <mpi.h>

#include "../utility/numeric_cast.hpp"
#include "../utility/print_utils.hpp"

#include "audited_routine_aliases.hpp"

namespace uit {

// TODO rename pid_t
using proc_id_t = int;

int get_nprocs(MPI_Comm comm=MPI_COMM_WORLD) {
  int res;
  UIT_Comm_size(
    comm,
    &res
  );
  return res;
}

//TODO replace with get_proc_id
proc_id_t get_rank(MPI_Comm comm=MPI_COMM_WORLD) {
  int res;
  UIT_Comm_rank(
    comm,
    &res
  );
  return res;
}

// TODO rename get_pid
proc_id_t get_proc_id(MPI_Comm comm=MPI_COMM_WORLD) {
  return get_rank(comm);
}

bool is_root(MPI_Comm comm=MPI_COMM_WORLD) { return get_rank(comm) == 0; }

bool is_multiprocess() { return get_nprocs() > 1; }

int get_count(const MPI_Status& status, const MPI_Datatype& datatype) {
  int res;
  UIT_Get_count(
    &status, // const MPI_Status * status: return status of receive operation
    datatype, // MPI_Datatype datatype: datatype of each receive buffer element
    &res // int *count: number of received elements (integer)
  );
  emp_assert( res != MPI_UNDEFINED );
  return res;
}

std::string to_string(const MPI_Status & status) {
  std::stringstream ss;
  ss << format_member(
    "MPI_Get_count",
    uit::get_count(status, MPI_BYTE)
  ) << std::endl;
  ss << format_member(
    "MPI_Test_cancelled",
    [&](){
      int res;
      MPI_Test_cancelled(&status, &res);
      return res;
    }()
  ) << std::endl;
  ss << format_member(
    "int MPI_SOURCE",
    (int) status.MPI_SOURCE
  ) << std::endl;
  ss << format_member(
    "int MPI_TAG",
    (int) status.MPI_TAG
  ) << std::endl;
  ss << format_member(
    "int MPI_ERROR",
    (int) status.MPI_ERROR
  );
  return ss.str();
}

MPI_Group comm_to_group(const MPI_Comm & comm){
  MPI_Group group;
  UIT_Comm_group(
    comm, // MPI_Comm comm
    &group // MPI_Group* group
  );
  return group;
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

size_t comm_size(const MPI_Comm & comm) {
  const int res{ get_nprocs(comm) };
  emp_assert(res >= 0);
  return res;
}

MPI_Comm split_comm(
  const std::function<int(const int)> colorer,
  MPI_Comm comm=MPI_COMM_WORLD
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

emp::vector<proc_id_t> get_comm_ranks(const MPI_Comm& comm) {
  return get_group_ranks(comm_to_group(comm));
}

void print_separator(const proc_id_t rank) {
  std::cout << "======================" << std::endl;
  std::cout << "\\/ \\/ Rank " << rank << " \\/ \\/" << std::endl;
  std::cout << "======================" << std::endl;
}

//TODO add a thread do successively and a combined do successively
template<
  typename TaskFunction,
  typename BeforeTaskFunction=std::function<void(const proc_id_t)>
>
void do_successively(
  TaskFunction&& task,
  BeforeTaskFunction&& before_task=[](const proc_id_t rank){},
  MPI_Comm comm=MPI_COMM_WORLD
) {
  for (proc_id_t rank = 0; rank < get_nprocs(comm); ++rank) {
    if (rank == get_rank(comm)) {
      before_task(rank);
      task();
    }
    UIT_Barrier(comm);
  }
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

proc_id_t translate_comm_rank(
  const proc_id_t rank,
  const MPI_Comm& from,
  const MPI_Comm& to=MPI_COMM_WORLD
) {
  return translate_group_rank(rank, comm_to_group(from), comm_to_group(to));
}

void init_multithread(int *argc, char ***argv) {
  int res{};
  UIT_Init_thread(argc, argv, MPI_THREAD_MULTIPLE, &res);
  assert(res >= MPI_THREAD_MULTIPLE);
}

void init_multithread() {
  int argc{};
  init_multithread(&argc, nullptr);
}

void mpi_init() {
  int argc{};
  UIT_Init(&argc, nullptr);
}

int combine_tag(const size_t a, const size_t b) {

  // half of non-sign int bits
  const size_t int_bits = sizeof(int) * 8;
  const size_t each_bits = (int_bits - 1) / 2;

  // bounds checking
  emp_assert(std::bitset<each_bits>(a).to_ullong() == a);
  emp_assert(std::bitset<each_bits>(b).to_ullong() == b);

  const auto bottom_bits = std::bitset<int_bits>(a);
  const auto top_bits = std::bitset<int_bits>(b) << each_bits;

  const auto res = top_bits | bottom_bits;

  return uit::numeric_cast<int>(res.to_ullong());

}

std::string get_name(const MPI_Comm& comm) {
  int len;
  char data[MPI_MAX_OBJECT_NAME];
  UIT_Comm_get_name(comm, data, &len);
  return std::string{}.assign(data, len);
}

std::string comm_to_string(const MPI_Comm& comm) {
  std::stringstream ss;
  ss << format_member(
    "uit::comm_size(comm)", uit::comm_size(comm)
  ) << std::endl;
  ss << format_member(
    "uit::get_comm_ranks(comm)", uit::to_string(uit::get_comm_ranks(comm))
  ) << std::endl;
  ss << format_member(
    "uit::get_name(comm)", uit::get_name(comm)
  ) << std::endl;
  return ss.str();
}

MPI_Comm duplicate_comm(const MPI_Comm comm) {
  MPI_Comm res;
  UIT_Comm_dup(
    comm,
    &res
  );
  return res;
}

bool test_completion(const MPI_Request& request) {
  int flag{};
  // on MPI spec, request is listed as an input param but not taken as const ptr
  UIT_Test(
    const_cast<MPI_Request*>(&request), // MPI_Request * request
    &flag, // int *flag
    MPI_STATUS_IGNORE // MPI_Status * status
  );
  return flag;
}

bool test_null(const MPI_Request& request) {
  return request == MPI_REQUEST_NULL;
}

} // namespace uit
