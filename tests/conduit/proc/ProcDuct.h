#include <chrono>
#include <thread>

#include "mpi.h"

#define CATCH_CONFIG_RUNNER
#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/config.h"
#include "utility/CircularIndex.h"
#include "distributed/mpi_utils.h"
#include "distributed/RDMAWindowManager.h"
#include "conduit/pipe_utils.h"
#include "utility/math_utils.h"

// TODO rewrite this to use catch and to use mesh_utils

uit::Outlet<int> make_input() {

  auto input_pipe = uit::make_pipe<int>();
  auto & [__, outlet] = input_pipe;

  const int source = uit::circular_index(
    uit::get_rank(),
    uit::get_nprocs(),
    -1
  );

  outlet.SplitDuct<uit::ProcOutletDuct<int>>(uit::get_rank(), source);

  return outlet;

}

uit::Inlet<int> make_output() {

  // another way of doing it
  auto inlet = uit::make_sink<int>();

  const int dest = uit::circular_index(uit::get_rank(), uit::get_nprocs(), 1);

  inlet.EmplaceDuct<uit::ProcInletDuct<int>>(uit::get_rank(), dest);

  return inlet;

}


TEST_CASE("Ring Mesh") {


  uit::Outlet<int> input = make_input();

  uit::Inlet<int> output = make_output();

  uit::RDMAWindowManager::Initialize();

  const int message = uit::get_rank();

  output.MaybePut(message);

  // std::this_thread::sleep_for(std::chrono::seconds{1});
  MPI_Barrier(MPI_COMM_WORLD);

  const int res = input.GetCurrent();

  REQUIRE(
    res
    == uit::numeric_cast<int>(
      uit::circular_index(uit::get_rank(), uit::get_nprocs(), -1)
    )
  );

}

int main(int argc, char* argv[]) {

  uit::verify(MPI_Init(&argc, &argv));

  int result = Catch::Session{}.run( argc, argv );

  uit::RDMAWindowManager::Cleanup(); // TODO rename Finalize

  uit::verify(MPI_Finalize());

  return result;
}
