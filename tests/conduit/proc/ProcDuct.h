#include <chrono>
#include <thread>

#include "mpi.h"

#define CATCH_CONFIG_RUNNER
#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/config.h"
#include "conduit/ImplSpec.h"
#include "utility/CircularIndex.h"
#include "distributed/mpi_utils.h"
#include "distributed/RDMAWindowManager.h"
#include "distributed/assign_utils.h"
#include "utility/math_utils.h"
#include "mesh/Mesh.h"
#include "mesh/MeshNodeInput.h"
#include "mesh/MeshNodeOutput.h"
#include "topology/DyadicTopologyFactory.h"
#include "topology/RingTopologyFactory.h"
#include "topology/ProConTopologyFactory.h"

#include "../../MultiprocessReporter.h"

TEST_CASE("Unmatched gets") {

  uit::Mesh<uit::ImplSpec<int>> mesh{
    uit::DyadicTopologyFactory{}(uit::get_nprocs()),
    uit::AssignIntegrated<uit::thread_id_t>{},
    uit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh(0);

  REQUIRE( bundles.size() == 1 );

  uit::Outlet<uit::ImplSpec<int>> input = bundles[0].GetInput(0);
  uit::Inlet<uit::ImplSpec<int>> output = bundles[0].GetOutput(0);

  output.MaybePut(42);
  // this barrier is necessary for RDMA... TODO why?
  uit::verify(MPI_Barrier(MPI_COMM_WORLD));
  std::this_thread::sleep_for(std::chrono::seconds{1});


  for (int i = 0; i <= 2 * DEFAULT_BUFFER; ++i) {
    REQUIRE( input.GetCurrent() == 42 );
  }

  uit::RDMAWindowManager::Cleanup();

}

TEST_CASE("Unmatched puts") {

  uit::Mesh<int> mesh{
    uit::DyadicMeshFactory<int>{}(uit::get_nprocs()),
    uit::AssignIntegrated<uit::thread_id_t>{},
    uit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh(0);

  REQUIRE( bundles.size() == 1 );

  uit::Outlet<int> input = bundles[0].inputs[0];
  uit::Inlet<int> output = bundles[0].outputs[0];

  uit::RDMAWindowManager::Initialize();

  for (int i = 0; i <= DEFAULT_BUFFER * 2; ++i) {
    output.MaybePut(i);
  }

  // this barrier is necessary for RDMA... TODO why?
  uit::verify(MPI_Barrier(MPI_COMM_WORLD));
  std::this_thread::sleep_for(std::chrono::seconds{1});

  // TODO why are these extra GetCurrent's necessary?
  input.GetCurrent();
  input.GetCurrent();
  REQUIRE( input.GetCurrent() >= DEFAULT_BUFFER - 1 );
  REQUIRE( input.GetCurrent() <= 2 * DEFAULT_BUFFER );

  uit::RDMAWindowManager::Cleanup();

}

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
  // this barrier is necessary for RDMA... TODO why?
  uit::verify(MPI_Barrier(MPI_COMM_WORLD));
  std::this_thread::sleep_for(std::chrono::seconds{1});

  // everyone should have gotten the final message by now
  REQUIRE( input.GetCurrent() == 2 * DEFAULT_BUFFER  );

  uit::RDMAWindowManager::Cleanup(); // TODO rename Finalize

}

std::pair<
  std::optional< uit::Outlet<int> >,
  std::optional< uit::Inlet<int> >
> make_producer_consumer_bundle() {

  uit::Mesh<int> mesh{
    uit::ProducerConsumerMeshFactory<int>{}(uit::get_nprocs()),
    uit::AssignIntegrated<uit::thread_id_t>{},
    uit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh(0);

  REQUIRE( bundles.size() == 1 );

  std::optional<uit::Outlet<int>> input = (
    bundles[0].inputs.size()
    ? std::optional{bundles[0].inputs[0]}
    : std::nullopt
  );
  std::optional<uit::Inlet<int>> output = (
    bundles[0].outputs.size()
    ? std::optional{bundles[0].outputs[0]}
    : std::nullopt
  );

  uit::RDMAWindowManager::Initialize();

  return std::pair{input, output};

};

TEST_CASE("Producer-Consumer Mesh") {

  auto [input, output] = make_producer_consumer_bundle();

  REQUIRE((input || output));

  uit::verify(MPI_Barrier(MPI_COMM_WORLD));

  // check that everyone's connected properly
  if (output) output->MaybePut(uit::get_rank());

  // give enough time to "guarantee" message delivery
  std::this_thread::sleep_for(std::chrono::seconds{1});
  // this barrier is necessary for RDMA... TODO why?
  uit::verify(MPI_Barrier(MPI_COMM_WORLD));

  // did we get expected rank ID as message?
  if (uit::get_nprocs() % 2 && uit::get_rank() + 1 == uit::get_nprocs()) {
    // is odd process loop at end
    REQUIRE( input->GetCurrent() == uit::get_rank());
  } else if (input) {
    // is consumer
    REQUIRE(
      input->GetCurrent() == uit::numeric_cast<int>(
        uit::circular_index(uit::get_rank(), uit::get_nprocs(), -1)
      )
    );
  } else REQUIRE( uit::get_rank() % 2 == 0 ); // is producer

  // setup for next test
  // this barrier is necessary for RDMA... TODO why?
  uit::verify(MPI_Barrier(MPI_COMM_WORLD));
  if (output) output->MaybePut(0);
  std::this_thread::sleep_for(std::chrono::seconds{1});

  // check that buffer wraparound works properly
  for (int i = 0; i <= DEFAULT_BUFFER * 2; ++i) {

    uit::verify(MPI_Barrier(MPI_COMM_WORLD));

    if (output) output->MaybePut(i);

    // nobody should see messages that haven't been sent yet
    if (input) REQUIRE(input->GetCurrent() <= i);

  }

  // give enough time to "guarantee" message delivery
  uit::verify(MPI_Barrier(MPI_COMM_WORLD));
  std::this_thread::sleep_for(std::chrono::seconds{1});

  // everyone should have gotten the final message by now
  if (input) REQUIRE( input->GetCurrent() == DEFAULT_BUFFER * 2 );


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
