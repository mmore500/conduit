#include <chrono>
#include <thread>

#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "../../MultiprocessReporter.hpp"

#include "conduit/config.hpp"
#include "conduit/ImplSpec.hpp"
#include "utility/CircularIndex.hpp"
#include "distributed/mpi_utils.hpp"
#include "distributed/RDMAWindowManager.hpp"
#include "distributed/assign_utils.hpp"
#include "utility/math_utils.hpp"
#include "mesh/Mesh.hpp"
#include "mesh/MeshNodeInput.hpp"
#include "mesh/MeshNodeOutput.hpp"
#include "distributed/MPIGuard.hpp"
#include "topology/DyadicTopologyFactory.hpp"
#include "topology/RingTopologyFactory.hpp"
#include "topology/ProConTopologyFactory.hpp"

#include "../../MultiprocessReporter.hpp"

const uit::MPIGuard guard;

using MSG_T = int;
using Spec = uit::ImplSpec<MSG_T, DEFAULT_BUFFER, ImplSel>;

TEST_CASE("Unmatched gets") {

  uit::Mesh<Spec> mesh{
    uit::DyadicTopologyFactory{}(uit::get_nprocs()),
    uit::AssignIntegrated<uit::thread_id_t>{},
    uit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh();

  REQUIRE( bundles.size() == 1 );

  uit::Outlet<Spec> input = bundles[0].GetInput(0);
  uit::Inlet<Spec> output = bundles[0].GetOutput(0);

  output.MaybePut(42);
  // this barrier is necessary for RDMA... TODO why?
  uit::verify(MPI_Barrier(MPI_COMM_WORLD));
  std::this_thread::sleep_for(std::chrono::seconds{1});


  for (MSG_T i = 0; i <= 2 * DEFAULT_BUFFER; ++i) {
    REQUIRE( input.GetCurrent() == 42 );
  }

}

TEST_CASE("Unmatched puts") {

  uit::Mesh<Spec> mesh{
    uit::DyadicTopologyFactory{}(uit::get_nprocs()),
    uit::AssignIntegrated<uit::thread_id_t>{},
    uit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh();

  REQUIRE( bundles.size() == 1 );

  uit::Outlet<Spec> input = bundles[0].GetInput(0);
  uit::Inlet<Spec> output = bundles[0].GetOutput(0);

  for (MSG_T i = 0; i <= DEFAULT_BUFFER * 2; ++i) {
    output.MaybePut(i);
  }

  // this barrier is necessary for RDMA... TODO why?
  uit::verify(MPI_Barrier(MPI_COMM_WORLD));
  std::this_thread::sleep_for(std::chrono::seconds{1});

  REQUIRE( input.GetCurrent() <= 2 * DEFAULT_BUFFER );

  // TODO why are these extra GetCurrent's necessary for IMsg?
  for(size_t i = 0; i < 10; ++i) input.GetCurrent();

  REQUIRE( input.GetCurrent() >= DEFAULT_BUFFER - 1 );
  REQUIRE( input.GetCurrent() <= 2 * DEFAULT_BUFFER );

}

TEST_CASE("Validity") {

  uit::Mesh<Spec> mesh{
    uit::DyadicTopologyFactory{}(uit::get_nprocs()),
    uit::AssignIntegrated<uit::thread_id_t>{},
    uit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh();

  REQUIRE( bundles.size() == 1 );

  uit::Outlet<Spec> input = bundles[0].GetInput(0);
  uit::Inlet<Spec> output = bundles[0].GetOutput(0);

  int last{};
  uit::verify(MPI_Barrier(MPI_COMM_WORLD));
  for (MSG_T msg = 0; msg < 10 * std::kilo{}.num; ++msg) {
    output.MaybePut(msg);
    const MSG_T current = input.GetCurrent();
    REQUIRE( current >= 0 );
    REQUIRE( current < 10 * std::kilo{}.num );
    REQUIRE( last <= current );
    last = current;
  }

  // all puts must be complete for next part of the test
  uit::verify(MPI_Barrier(MPI_COMM_WORLD));

  // flush
  for (size_t i = 0; i < std::kilo{}.num; ++i) input.GetCurrent();

  for (size_t i = 0; i < 10 * std::kilo{}.num; ++i) {
    REQUIRE( input.GetCurrent() >= 0 );
    REQUIRE( input.GetCurrent() == input.GetCurrent() );
  }

}

decltype(auto) make_ring_bundle() {
  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(uit::get_nprocs()),
    uit::AssignIntegrated<uit::thread_id_t>{},
    uit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh();

  REQUIRE( bundles.size() == 1);

  uit::Outlet<Spec> input = bundles[0].GetInput(0);
  uit::Inlet<Spec> output = bundles[0].GetOutput(0);

  return std::tuple{output, input};

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
    input.GetNext()
    == uit::numeric_cast<MSG_T>(
      uit::circular_index(uit::get_rank(), uit::get_nprocs(), -1)
    )
  );

  // setup for next test
  output.MaybePut(0);
  uit::verify(MPI_Barrier(MPI_COMM_WORLD));
  std::this_thread::sleep_for(std::chrono::seconds{1});

  // check that buffer wraparound works properly
  for (MSG_T i = 0; i <= 2 * DEFAULT_BUFFER; ++i) {

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

}

std::pair<
  std::optional< uit::Outlet<Spec> >,
  std::optional< uit::Inlet<Spec> >
> make_producer_consumer_bundle() {

  uit::Mesh<Spec> mesh{
    uit::ProConTopologyFactory{}(uit::get_nprocs()),
    uit::AssignIntegrated<uit::thread_id_t>{},
    uit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh(0);

  REQUIRE( bundles.size() == 1 );

  std::optional<uit::MeshNodeInput<Spec>> input = (
    bundles[0].HasInputs()
    ? std::optional{bundles[0].GetInput(0)}
    : std::nullopt
  );
  std::optional<uit::MeshNodeOutput<Spec>> output = (
    bundles[0].HasOutputs()
    ? std::optional{bundles[0].GetOutput(0)}
    : std::nullopt
  );

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
  for (MSG_T i = 0; i <= DEFAULT_BUFFER * 2; ++i) {

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

}
