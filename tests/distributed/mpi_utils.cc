#include "mpi.h"

#define CATCH_CONFIG_RUNNER
#include "Catch/single_include/catch2/catch.hpp"

#include "distributed/mpi_utils.h"
#include "utility/math_utils.h"

#include "utility/assign_utils.h"

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

  const int my_rank{ get_rank() };

  const int num_ranks{ get_nprocs() };

  const MPI_Group every_other{ comm_to_group(
    split_comm(assign_round_robin<proc_id_t>(2))
  ) };
  const MPI_Group halves{ comm_to_group(
    split_comm(assign_contiguously<proc_id_t>(2, num_ranks))
  ) };
  const MPI_Group quarters{ comm_to_group(
    split_comm(assign_contiguously<proc_id_t>(4, num_ranks))
  ) };

  REQUIRE(
    group_size(intersect_groups({halves, quarters})) == group_size(quarters)
  );

  if (num_ranks >= 4) REQUIRE(
    group_size(intersect_groups({halves, every_other})) < group_size(halves)
  );

  if (num_ranks >= 4) REQUIRE(
    group_size(intersect_groups({halves, every_other}))
    < group_size(every_other)
  );

  const emp::vector<proc_id_t> halves_quarters_ranks(
    get_group_ranks(intersect_groups({halves, quarters}))
  );

  REQUIRE( std::all_of(
    std::begin(halves_quarters_ranks),
    std::end(halves_quarters_ranks),
    [=](const auto & rank){
      return assign_contiguously<proc_id_t>(2, num_ranks)(rank)
        == assign_contiguously<proc_id_t>(2, num_ranks)(my_rank);
    }
  ) );

  REQUIRE( std::all_of(
    std::begin(halves_quarters_ranks),
    std::end(halves_quarters_ranks),
    [=](const auto & rank){
      return assign_contiguously<proc_id_t>(4, num_ranks)(rank)
        == assign_contiguously<proc_id_t>(4, num_ranks)(my_rank);
    }
  ) );

  const emp::vector<proc_id_t> halves_every_other_ranks(
    get_group_ranks(intersect_groups({every_other, halves}))
  );

  REQUIRE( std::all_of(
    std::begin(halves_every_other_ranks),
    std::end(halves_every_other_ranks),
    [=](const auto & rank){
      return assign_round_robin<proc_id_t>(2)(rank) == assign_round_robin<proc_id_t>(2)(my_rank);
    }
  ) );

  REQUIRE( std::all_of(
    std::begin(halves_every_other_ranks),
    std::end(halves_every_other_ranks),
    [=](const auto & rank){
      return assign_contiguously<proc_id_t>(2, num_ranks)(rank)
        == assign_contiguously<proc_id_t>(2, num_ranks)(my_rank);
    }
  ) );


}

TEST_CASE("combine_groups") {

  const proc_id_t my_rank{ get_rank() };

  const proc_id_t num_ranks{ get_nprocs() };

  const MPI_Group every_other{ comm_to_group(
    split_comm(assign_round_robin<proc_id_t>(2))
  ) };
  const MPI_Group halves{ comm_to_group(
    split_comm(assign_contiguously<proc_id_t>(2, num_ranks))
  ) };
  const MPI_Group quarters{ comm_to_group(
    split_comm(assign_contiguously<proc_id_t>(4, num_ranks))
  ) };

  REQUIRE(
    group_size(combine_groups({halves, quarters})) == group_size(halves)
  );

  if (num_ranks >= 4) REQUIRE(
    group_size(combine_groups({halves, every_other})) > group_size(halves)
  );

  const MPI_Comm combo_comm{
    group_to_comm(combine_groups({halves, every_other}))
  };

  const emp::vector<proc_id_t> comm_ranks( get_comm_ranks(combo_comm) );

  if (num_ranks >= 4) REQUIRE( std::any_of(
    std::begin(comm_ranks),
    std::end(comm_ranks),
    [my_rank](const auto & rank){
      return assign_round_robin<proc_id_t>(2)(rank) != assign_round_robin<proc_id_t>(2)(my_rank);
    }
  ) );

  if (num_ranks >= 4) REQUIRE( std::any_of(
    std::begin(comm_ranks),
    std::end(comm_ranks),
    [my_rank](const auto & rank){
      return assign_round_robin<proc_id_t>(2)(rank) == assign_round_robin<proc_id_t>(2)(my_rank);
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

  const proc_id_t my_rank{ get_rank() };

  const proc_id_t num_ranks{ get_nprocs() };

  const MPI_Comm every_other{
    split_comm([](const proc_id_t rank){ return rank%2 == 0; })
  };
  const MPI_Comm halves{
    split_comm([=](const proc_id_t rank){ return rank * 2 / num_ranks; })
  };

  REQUIRE( difference(comm_size(every_other), comm_size(halves)) <= 1 );

  emp::vector<proc_id_t> comm_ranks( comm_size(every_other) );

  verify(MPI_Allgather(
    &my_rank, // const void *sendbuf
    1, // int sendcount
    MPI_INT, // MPI_Datatype sendtype
    comm_ranks.data(), // void *recvbuf
    1, // int recvcount
    MPI_INT, // MPI_Datatype recvtype
    every_other // MPI_Comm comm
  ));

  REQUIRE( std::all_of(
    std::begin(comm_ranks),
    std::end(comm_ranks),
    [my_rank](const auto & rank){ return rank % 2 == my_rank % 2; }
  ) );

  comm_ranks.resize( comm_size(halves) );

  verify(MPI_Allgather(
    &my_rank, // const void *sendbuf
    1, // int sendcount
    MPI_INT, // MPI_Datatype sendtype
    comm_ranks.data(), // void *recvbuf
    1, // int recvcount
    MPI_INT, // MPI_Datatype recvtype
    halves // MPI_Comm comm
  ));

  REQUIRE( std::all_of(
    std::begin(comm_ranks),
    std::end(comm_ranks),
    [=](const auto & rank){
      return rank * 2 / num_ranks == my_rank * 2 / num_ranks;
    }
  ) );

}

TEST_CASE("do_successively") {

  do_successively(
    [=](){ std::cout << "hello" << std::endl; },
    print_separator
  );

}

int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  int result = Catch::Session{}.run( argc, argv );

  MPI_Finalize();

  return result;
}
