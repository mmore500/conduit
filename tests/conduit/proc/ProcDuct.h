#include <chrono>
#include <thread>

#include "mpi.h"

#define CATCH_CONFIG_RUNNER
#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/config.h"
#include "utility/CircularIndex.h"
#include "distributed/mpi_utils.h"
#include "distributed/RDMAWindowManager.h"
#include "distributed/assign_utils.h"
#include "conduit/pipe_utils.h"
#include "utility/math_utils.h"
#include "mesh/Mesh.h"
#include "mesh/mesh_utils.h"

TEST_CASE("Ring Mesh") {


  uit::Mesh<int> mesh{
    uit::RingMeshFactory<int>{}(uit::get_nprocs()),
    uit::AssignIntegrated<uit::thread_id_t>{},
    uit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh(0);

  REQUIRE( bundles.size() == 1);

  uit::Outlet<int> input = bundles[0].inputs[0];
  uit::Inlet<int> output = bundles[0].outputs[0];

  uit::RDMAWindowManager::Initialize();

  const int message = uit::get_rank();

  output.MaybePut(message);

  // std::this_thread::sleep_for(std::chrono::seconds{1});
  MPI_Barrier(MPI_COMM_WORLD);

  REQUIRE(
    input.GetCurrent()
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
