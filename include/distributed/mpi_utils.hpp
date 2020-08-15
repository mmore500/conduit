#pragma once

#include <assert.h>
#include <limits>
#include <numeric>
#include <stddef.h>

#include <mpi.h>

#include "../utility/numeric_cast.hpp"
#include "../utility/print_utils.hpp"

namespace uit {

// TODO rename pid_t
using proc_id_t = int;

void verify(const int err) {

  std::string name;

  switch(err) {

    case MPI_SUCCESS:
      name = "MPI_SUCCESS";
      break;
    case MPI_ERR_BUFFER:
      name = "MPI_ERR_BUFFER";
      break;
    case MPI_ERR_COUNT:
      name = "MPI_ERR_COUNT";
      break;
    case MPI_ERR_TYPE:
      name = "MPI_ERR_TYPE";
      break;
    case MPI_ERR_TAG:
      name = "MPI_ERR_TAG";
      break;
    case MPI_ERR_COMM:
      name = "MPI_ERR_COMM";
      break;
    case MPI_ERR_RANK:
      name = "MPI_ERR_RANK";
      break;
    case MPI_ERR_ROOT:
      name = "MPI_ERR_ROOT";
      break;
    case MPI_ERR_GROUP:
      name = "MPI_ERR_GROUP";
      break;
    case MPI_ERR_OP:
      name = "MPI_ERR_OP";
      break;
    case MPI_ERR_TOPOLOGY:
      name = "MPI_ERR_TOPOLOGY";
      break;
    case MPI_ERR_DIMS:
      name = "MPI_ERR_DIMS";
      break;
    case MPI_ERR_ARG:
      name = "MPI_ERR_ARG";
      break;
    case MPI_ERR_UNKNOWN:
      name = "MPI_ERR_UNKNOWN";
      break;
    case MPI_ERR_TRUNCATE:
      name = "MPI_ERR_TRUNCATE";
      break;
    case MPI_ERR_OTHER:
      name = "MPI_ERR_OTHER";
      break;
    case MPI_ERR_INTERN:
      name = "MPI_ERR_INTERN";
      break;
    case MPI_ERR_IN_STATUS:
      name = "MPI_ERR_IN_STATUS";
      break;
    case MPI_ERR_PENDING:
      name = "MPI_ERR_PENDING";
      break;
    case MPI_ERR_REQUEST:
      name = "MPI_ERR_REQUEST";
      break;
    case MPI_ERR_ACCESS:
      name = "MPI_ERR_ACCESS";
      break;
    case MPI_ERR_AMODE:
      name = "MPI_ERR_AMODE";
      break;
    case MPI_ERR_BAD_FILE:
      name = "MPI_ERR_BAD_FILE";
      break;
    case MPI_ERR_CONVERSION:
      name = "MPI_ERR_CONVERSION";
      break;
    case MPI_ERR_DUP_DATAREP:
      name = "MPI_ERR_DUP_DATAREP";
      break;
    case MPI_ERR_FILE_EXISTS:
      name = "MPI_ERR_FILE_EXISTS";
      break;
    case MPI_ERR_FILE_IN_USE:
      name = "MPI_ERR_FILE_IN_USE";
      break;
    case MPI_ERR_FILE:
      name = "MPI_ERR_FILE";
      break;
    case MPI_ERR_INFO:
      name = "MPI_ERR_INFO";
      break;
    case MPI_ERR_INFO_KEY:
      name = "MPI_ERR_INFO_KEY";
      break;
    case MPI_ERR_INFO_VALUE:
      name = "MPI_ERR_INFO_VALUE";
      break;
    case MPI_ERR_INFO_NOKEY:
      name = "MPI_ERR_INFO_NOKEY";
      break;
    case MPI_ERR_IO:
      name = "MPI_ERR_IO";
      break;
    case MPI_ERR_NAME:
      name = "MPI_ERR_NAME";
      break;
    case MPI_ERR_NO_MEM:
      name = "MPI_ERR_NO_MEM";
      break;
    case MPI_ERR_NOT_SAME:
      name = "MPI_ERR_NOT_SAME";
      break;
    case MPI_ERR_NO_SPACE:
      name = "MPI_ERR_NO_SPACE";
      break;
    case MPI_ERR_NO_SUCH_FILE:
      name = "MPI_ERR_NO_SUCH_FILE";
      break;
    case MPI_ERR_PORT:
      name = "MPI_ERR_PORT";
      break;
    case MPI_ERR_QUOTA:
      name = "MPI_ERR_QUOTA";
      break;
    case MPI_ERR_READ_ONLY:
      name = "MPI_ERR_READ_ONLY";
      break;
    case MPI_ERR_SERVICE:
      name = "MPI_ERR_SERVICE";
      break;
    case MPI_ERR_SPAWN:
      name = "MPI_ERR_SPAWN";
      break;
    case MPI_ERR_UNSUPPORTED_DATAREP:
      name = "MPI_ERR_UNSUPPORTED_DATAREP";
      break;
    case MPI_ERR_UNSUPPORTED_OPERATION:
      name = "MPI_ERR_UNSUPPORTED_OPERATION";
      break;
    case MPI_ERR_WIN:
      name = "MPI_ERR_WIN";
      break;
    case MPI_ERR_BASE:
      name = "MPI_ERR_BASE";
      break;
    case MPI_ERR_LOCKTYPE:
      name = "MPI_ERR_LOCKTYPE";
      break;
    case MPI_ERR_KEYVAL:
      name = "MPI_ERR_KEYVAL";
      break;
    case MPI_ERR_RMA_CONFLICT:
      name = "MPI_ERR_RMA_CONFLICT";
      break;
    case MPI_ERR_RMA_SYNC:
      name = "MPI_ERR_RMA_SYNC";
      break;
    case MPI_ERR_SIZE:
      name = "MPI_ERR_SIZE";
      break;
    case MPI_ERR_DISP:
      name = "MPI_ERR_DISP";
      break;
    case MPI_ERR_ASSERT:
      name = "MPI_ERR_ASSERT";
      break;
    case MPI_ERR_LASTCODE:
      name = "MPI_ERR_LASTCODE";
      break;
    default:
      name = "unknown";

  }

  emp_assert(err == 0, err, name);

}

int get_nprocs(MPI_Comm comm=MPI_COMM_WORLD) {
  int res;
  verify(MPI_Comm_size(
    comm,
    &res
  ));
  return res;
}

//TODO replace with get_proc_id
proc_id_t get_rank(MPI_Comm comm=MPI_COMM_WORLD) {
  int res;
  verify(MPI_Comm_rank(
    comm,
    &res
  ));
  return res;
}

// TODO rename get_pid
proc_id_t get_proc_id(MPI_Comm comm=MPI_COMM_WORLD) {
  return get_rank(comm);
}

bool is_root(MPI_Comm comm=MPI_COMM_WORLD) { return get_rank(comm) == 0; }

bool is_multiprocess() { return get_nprocs() > 1; }

std::string to_string(const MPI_Status & status) {
  std::stringstream ss;
  ss << format_member(
    "MPI_Get_count",
    [&](){
      int res;
      MPI_Get_count(&status, MPI_BYTE, &res);
      return res;
    }()
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
  verify(MPI_Comm_group(
    comm, // MPI_Comm comm
    &group // MPI_Group* group
  ));
  return group;
}

MPI_Comm group_to_comm(
  const MPI_Group & group,
  const MPI_Comm & comm=MPI_COMM_WORLD
) {
  MPI_Comm res;
  verify(MPI_Comm_create_group(
    comm, // MPI_Comm comm
    group, // MPI_Group group
    0, // int tag TODO is this important?
    &res // MPI_Comm * newcomm
  ));
  return res;
}

MPI_Group intersect_groups(emp::vector<MPI_Group> groups) {

  MPI_Group res{
    groups.size() ? groups.back() : MPI_GROUP_EMPTY
  };

  // TODO more MPI_Group_free ?
  for ( ; groups.size(); groups.pop_back()) {

    MPI_Group temp;
    verify(MPI_Group_intersection(
      res, // MPI_Group group1,
      groups.back(), // MPI_Group group2,
      &temp // MPI_Group* newgroup
    ));
    verify(MPI_Group_free(
      &res // MPI_Group* group
    ));
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
    verify(MPI_Group_union(
      res, // MPI_Group group1
      groups.back(), // MPI_Group group2
      &temp // MPI_Group* newgroup
    ));
    verify(MPI_Group_free(
      &res // MPI_Group* group
    ));
    res = temp;
  }

  return res;

}

MPI_Group subtract_groups(
  const MPI_Group & minuend,
  const MPI_Group & subtrahend
) {

  MPI_Group res;

  verify(MPI_Group_difference(
    minuend, // MPI_Group group1
    subtrahend, // MPI_Group group2
    &res // MPI_Group * newgroup
  ));

  return res;

}

size_t group_size(const MPI_Group & group) {
  int res;
  verify(MPI_Group_size(
    group, // MPI_Group group
    &res // int *size
  ));
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
  verify(MPI_Group_translate_ranks(
    group, // MPI_Group group1
    within_group_ranks.size(), // int n
    within_group_ranks.data(), // const int ranks1[]
    comm_to_group(MPI_COMM_WORLD), // MPI_Group group2
    within_world_ranks.data() // int ranks2[]
  ));

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
    verify(MPI_Barrier(comm));
  }
}

#ifndef MPICH_VERSION
// apparently MPICH can't differentiate groups and comms?
proc_id_t translate_rank(
  const proc_id_t rank,
  const MPI_Group& from,
  const MPI_Group& to=comm_to_group(MPI_COMM_WORLD)
) {
  proc_id_t res;
  verify(MPI_Group_translate_ranks(
    from, // MPI_Group group1
    1, // int n
    &rank, // const int ranks1[]
    to, // MPI_Group group2
    &res // int ranks2[]
  ));
  return res;
}
#endif

proc_id_t translate_rank(
  const proc_id_t rank,
  const MPI_Comm& from,
  const MPI_Comm& to=MPI_COMM_WORLD
) {
  return translate_rank(rank, comm_to_group(from), comm_to_group(to));
}

void init_multithread(int *argc, char ***argv) {
  int res{};
  uit::verify(MPI_Init_thread(argc, argv, MPI_THREAD_MULTIPLE, &res));
  assert(res >= MPI_THREAD_MULTIPLE);
}

void init_multithread() {
  int argc{};
  init_multithread(&argc, nullptr);
}

void mpi_init() {
  int argc{};
  uit::verify(MPI_Init(&argc, nullptr));
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

std::string to_string(const MPI_Comm comm) {
  int len;
  char data[MPI_MAX_OBJECT_NAME];
  uit::verify(MPI_Comm_get_name(comm, data, &len));
  return std::string{}.assign(data, len);
}


MPI_Comm duplicate_comm(const MPI_Comm comm) {
  MPI_Comm res;
  uit::verify(MPI_Comm_dup(
    comm,
    &res
  ));
  return res;
}

bool test_completion(MPI_Request& request) {
  int flag{};
  uit::verify(MPI_Test(
    &request,
    &flag,
    MPI_STATUS_IGNORE
  ));
  return flag;
}

}
