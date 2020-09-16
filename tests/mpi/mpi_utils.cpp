#include <unordered_set>

#include <mpi.h>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/mpi/MpiGuard.hpp"
#include "uit/mpi/mpi_utils.hpp"
#include "uit/debug/MultiprocessReporter.hpp"
#include "uit/mpi/Request.hpp"
#include "uit/utility/assign_utils.hpp"
#include "uit/utility/math_utils.hpp"

const uit::MpiGuard guard;

TEST_CASE("get_nprocs") {

  // TODO
  // int get_nprocs(MPI_Comm comm=MPI_COMM_WORLD)

}

TEST_CASE("get_rank and get_proc_id") {

  // TODO
  // proc_id_t get_rank(MPI_Comm comm=MPI_COMM_WORLD)

  // TODO
  // proc_id_t get_proc_id(MPI_Comm comm=MPI_COMM_WORLD)

}

TEST_CASE("is_root") {

  // TODO
  // bool is_root(MPI_Comm comm=MPI_COMM_WORLD)

}

TEST_CASE("is_multiprocess") {

  // TODO
  // bool is_multiprocess()

}

TEST_CASE("comm_to_group") {

  // TODO
  // MPI_Group comm_to_group(const MPI_Comm & comm)

}

TEST_CASE("group_to_comm") {

  // TODO
  // MPI_Comm group_to_comm(
  //   const MPI_Group & group,
  //   const MPI_Comm & comm=MPI_COMM_WORLD
  // )

}

TEST_CASE("intersect_groups") {

  const int my_rank{ uit::get_rank() };

  const int num_ranks{ uit::get_nprocs() };

  const MPI_Group every_other{ uit::comm_to_group(
    uit::split_comm(uit::AssignRoundRobin<uit::proc_id_t>{2})
  ) };
  const MPI_Group halves{ uit::comm_to_group(
    uit::split_comm(uit::AssignContiguously<uit::proc_id_t>{
      2,
      uit::numeric_cast<size_t>(num_ranks)
    })
  ) };
  const MPI_Group quarters{ uit::comm_to_group(
    uit::split_comm(uit::AssignContiguously<uit::proc_id_t>{
      4,
      uit::numeric_cast<size_t>(num_ranks)
    })
  ) };

  REQUIRE(
    uit::group_size(uit::intersect_groups({halves, quarters}))
      == uit::group_size(quarters)
  );

  if (num_ranks >= 4) REQUIRE(
    uit::group_size(uit::intersect_groups({halves, every_other}))
      < uit::group_size(halves)
  );

  if (num_ranks >= 4) REQUIRE(
    uit::group_size(uit::intersect_groups({halves, every_other}))
      < uit::group_size(every_other)
  );

  const emp::vector<uit::proc_id_t> halves_quarters_ranks(
    uit::get_group_ranks(uit::intersect_groups({halves, quarters}))
  );

  REQUIRE( std::all_of(
    std::begin(halves_quarters_ranks),
    std::end(halves_quarters_ranks),
    [=](const auto & rank){
      return uit::AssignContiguously<uit::proc_id_t>{
        2,
        uit::numeric_cast<size_t>(num_ranks)
      }(rank) == uit::AssignContiguously<uit::proc_id_t>{
        2,
        uit::numeric_cast<size_t>(num_ranks)
      }(my_rank);
    }
  ) );

  REQUIRE( std::all_of(
    std::begin(halves_quarters_ranks),
    std::end(halves_quarters_ranks),
    [=](const auto & rank){
      return uit::AssignContiguously<uit::proc_id_t>{
        4,
        uit::numeric_cast<size_t>(num_ranks)
      }(rank) == uit::AssignContiguously<uit::proc_id_t>{
        4,
        uit::numeric_cast<size_t>(num_ranks)
      }(my_rank);
    }
  ) );

  const emp::vector<uit::proc_id_t> halves_every_other_ranks(
    uit::get_group_ranks(uit::intersect_groups({every_other, halves}))
  );

  REQUIRE( std::all_of(
    std::begin(halves_every_other_ranks),
    std::end(halves_every_other_ranks),
    [=](const auto & rank){
      return uit::AssignRoundRobin<uit::proc_id_t>{2}(rank) == uit::AssignRoundRobin<uit::proc_id_t>{2}(my_rank);
    }
  ) );

  REQUIRE( std::all_of(
    std::begin(halves_every_other_ranks),
    std::end(halves_every_other_ranks),
    [=](const auto & rank){
      return uit::AssignContiguously<uit::proc_id_t>{
        2,
        uit::numeric_cast<size_t>(num_ranks)
      }(rank) == uit::AssignContiguously<uit::proc_id_t>{
        2,
        uit::numeric_cast<size_t>(num_ranks)
      }(my_rank);
    }
  ) );


}

TEST_CASE("combine_groups") {

  const uit::proc_id_t my_rank{ uit::get_rank() };

  const uit::proc_id_t num_ranks{ uit::get_nprocs() };

  const MPI_Group every_other{ uit::comm_to_group(
    uit::split_comm(uit::AssignRoundRobin<uit::proc_id_t>{2})
  ) };
  const MPI_Group halves{ uit::comm_to_group(
    uit::split_comm(uit::AssignContiguously<uit::proc_id_t>{
      2,
      uit::numeric_cast<size_t>(num_ranks)
    })
  ) };
  const MPI_Group quarters{ uit::comm_to_group(
    uit::split_comm(uit::AssignContiguously<uit::proc_id_t>{
      4,
      uit::numeric_cast<size_t>(num_ranks)
    })
  ) };

  REQUIRE(
    uit::group_size(uit::combine_groups({halves, quarters}))
      == uit::group_size(halves)
  );

  if (num_ranks >= 4) REQUIRE(
    uit::group_size(uit::combine_groups({halves, every_other}))
      > uit::group_size(halves)
  );

  const emp::vector<uit::proc_id_t> group_ranks(uit:: get_group_ranks(
    uit::combine_groups({halves, every_other})
  ) );

  if (num_ranks >= 4) REQUIRE( std::any_of(
    std::begin(group_ranks),
    std::end(group_ranks),
    [my_rank](const auto & rank){
      return uit::AssignRoundRobin<uit::proc_id_t>{2}(rank) != uit::AssignRoundRobin<uit::proc_id_t>{2}(my_rank);
    }
  ) );

  if (num_ranks >= 4) REQUIRE( std::any_of(
    std::begin(group_ranks),
    std::end(group_ranks),
    [my_rank](const auto & rank){
      return uit::AssignRoundRobin<uit::proc_id_t>{2}(rank) == uit::AssignRoundRobin<uit::proc_id_t>{2}(my_rank);
    }
  ) );

}

TEST_CASE("subtract_groups") {

  // TODO
  // MPI_Group subtract_groups(
  //   const MPI_Group & minuend,
  //   const MPI_Group & subtrahend
  // )

}

TEST_CASE("group_size") {

  // TODO
  // size_t group_size(const MPI_Group & group)

}

TEST_CASE("comm_size") {

  // TODO
  // size_t comm_size(const MPI_Comm & comm)

}

TEST_CASE("split_comm") {

  const uit::proc_id_t my_rank{ uit::get_rank() };

  const uit::proc_id_t num_ranks{ uit::get_nprocs() };

  // TODO refactor to use assign_utils
  const MPI_Comm every_other{
    uit::split_comm([](const uit::proc_id_t rank){ return rank%2 == 0; })
  };
  const MPI_Comm halves{
    uit::split_comm([=](const uit::proc_id_t rank){
      return rank * 2 / num_ranks;
    })
  };

  REQUIRE(
    uit::difference(uit::comm_size(every_other), uit::comm_size(halves)) <= 1
  );

  emp::vector<uit::proc_id_t> comm_ranks( uit::comm_size(every_other) );

  UIT_Allgather(
    &my_rank, // const void *sendbuf
    1, // int sendcount
    MPI_INT, // MPI_Datatype sendtype
    comm_ranks.data(), // void *recvbuf
    1, // int recvcount
    MPI_INT, // MPI_Datatype recvtype
    every_other // MPI_Comm comm
  );

  REQUIRE( std::all_of(
    std::begin(comm_ranks),
    std::end(comm_ranks),
    [my_rank](const auto & rank){ return rank % 2 == my_rank % 2; }
  ) );

  comm_ranks.resize( uit::comm_size(halves) );

  UIT_Allgather(
    &my_rank, // const void *sendbuf
    1, // int sendcount
    MPI_INT, // MPI_Datatype sendtype
    comm_ranks.data(), // void *recvbuf
    1, // int recvcount
    MPI_INT, // MPI_Datatype recvtype
    halves // MPI_Comm comm
  );

  REQUIRE( std::all_of(
    std::begin(comm_ranks),
    std::end(comm_ranks),
    [=](const auto & rank){
      return rank * 2 / num_ranks == my_rank * 2 / num_ranks;
    }
  ) );

}

TEST_CASE("comm_to_string") {

  REQUIRE(!uit::comm_to_string(MPI_COMM_WORLD).empty());

}

TEST_CASE("test_null") {

  uit::Request req;

  REQUIRE( uit::test_null(req) );

  char buf;
  UIT_Irecv(
    &buf, // void *buf
    1, // int count
    MPI_BYTE, // MPI_Datatype datatype
    MPI_ANY_SOURCE, // int source
    MPI_ANY_TAG, // int tag
    MPI_COMM_WORLD, // MPI_Comm comm
    &req // MPI_Request *request
  );

  REQUIRE( !uit::test_null(req) );

  UIT_Cancel(&req);
  UIT_Request_free(&req);

  REQUIRE( uit::test_null(req) );

}
