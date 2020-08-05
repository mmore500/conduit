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

#include "../../MultiprocessReporter.h"

uit::Conduit<int> make_ring_bundle() {
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

  return uit::Conduit<int>{output, input};

}

TEST_CASE("Ring Mesh") {

  auto [output, input] = make_ring_bundle();

  uit::verify(MPI_Barrier(MPI_COMM_WORLD));

  // check that everyone's connected properly
  output.MaybePut(uit::get_rank());

  // give enough time to "guarantee" message delivery
  std::this_thread::sleep_for(std::chrono::seconds{1});
  // this barrier is necessary for RDMA... TODO why?
  uit::verify(MPI_Barrier(MPI_COMM_WORLD));

  REQUIRE(
    input.GetCurrent()
    == uit::numeric_cast<int>(
      uit::circular_index(uit::get_rank(), uit::get_nprocs(), -1)
    )
  );

  // setup for next test
  uit::verify(MPI_Barrier(MPI_COMM_WORLD));
  output.MaybePut(0);
  std::this_thread::sleep_for(std::chrono::seconds{1});

  // check that buffer wraparound works properly
  for (int i = 0; i <= 2 * DEFAULT_BUFFER; ++i) {

    output.MaybePut(i);

    // nobody should see messages that haven't been sent yet
    REQUIRE(input.GetCurrent() <= i);

    uit::verify(MPI_Barrier(MPI_COMM_WORLD));

  }

  // give enough time to "guarantee" message delivery
  uit::verify(MPI_Barrier(MPI_COMM_WORLD));
  std::this_thread::sleep_for(std::chrono::seconds{1});

  // everyone should have gotten the final message by now
  REQUIRE( input.GetCurrent() == 2 * DEFAULT_BUFFER  );

  uit::RDMAWindowManager::Cleanup(); // TODO rename Finalize

}

int main(int argc, char* argv[]) {

  uit::verify(MPI_Init(&argc, &argv));

  Catch::Session session;
  session.configData().reporterName = "multiprocess";
  int result = session.run( argc, argv );

  uit::verify(MPI_Finalize());

  return result;
}
