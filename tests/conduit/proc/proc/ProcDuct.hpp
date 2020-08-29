#include <ratio>
#include <thread>

#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/ImplSpec.hpp"
#include "distributed/assign_utils.hpp"
#include "distributed/MPIGuard.hpp"
#include "distributed/mpi_utils.hpp"
#include "distributed/MultiprocessReporter.hpp"
#include "distributed/RDMAWindowManager.hpp"
#include "mesh/Mesh.hpp"
#include "mesh/MeshNodeInput.hpp"
#include "mesh/MeshNodeOutput.hpp"
#include "topology/DyadicTopologyFactory.hpp"
#include "topology/ProConTopologyFactory.hpp"
#include "topology/RingTopologyFactory.hpp"
#include "utility/CircularIndex.hpp"
#include "utility/math_utils.hpp"

const uit::MPIGuard guard;

using MSG_T = int;
using Spec = uit::ImplSpec<MSG_T, DEFAULT_BUFFER, ImplSel>;

#define REPEAT for (size_t rep = 0; rep < std::deca{}.num; ++rep)

decltype(auto) make_dyadic_bundle() {

  uit::Mesh<Spec> mesh{
    uit::DyadicTopologyFactory{}(uit::get_nprocs()),
    uit::AssignIntegrated<uit::thread_id_t>{},
    uit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh();
  REQUIRE( bundles.size() == 1 );

  return std::tuple{ bundles[0].GetInput(0), bundles[0].GetOutput(0) };

};

decltype(auto) make_producer_consumer_bundle() {

  uit::Mesh<Spec> mesh{
    uit::ProConTopologyFactory{}(uit::get_nprocs()),
    uit::AssignIntegrated<uit::thread_id_t>{},
    uit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh(0);

  REQUIRE( bundles.size() == 1 );
  REQUIRE(
    (bundles[0].GetInputOrNullopt(0) || bundles[0].GetOutputOrNullopt(0))
  );

  return std::tuple{
    bundles[0].GetInputOrNullopt(0),
    bundles[0].GetOutputOrNullopt(0)
  };

};

decltype(auto) make_ring_bundle() {
  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(uit::get_nprocs()),
    uit::AssignIntegrated<uit::thread_id_t>{},
    uit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh();

  REQUIRE( bundles.size() == 1);

  return std::tuple{ bundles[0].GetInput(0), bundles[0].GetOutput(0) };

}

TEST_CASE("Unmatched gets") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  for (MSG_T i = 0; i <= 2 * DEFAULT_BUFFER; ++i) {
    REQUIRE( input.GetCurrent() == 0 );
  }

  uit::verify(MPI_Barrier( MPI_COMM_WORLD ));

  output.SurePut(42);
  REQUIRE( input.GetNext() == 42 );

  for (MSG_T i = 0; i <= 2 * DEFAULT_BUFFER; ++i) {
    REQUIRE( input.GetCurrent() == 42 );
  }

} }

TEST_CASE("Unmatched puts") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  for (MSG_T i = 0; i <= 2 * DEFAULT_BUFFER; ++i) output.TryPut(i);

  if (input.GetCurrent() > 2 * DEFAULT_BUFFER) {
    std::cout << rep << std::endl;
    std::cout << input.GetNetFlux() << std::endl;
  }
  REQUIRE( input.GetCurrent() <= 2 * DEFAULT_BUFFER );

  uit::verify(MPI_Barrier( MPI_COMM_WORLD )); // todo why

} }

TEST_CASE("Eventual flush-out") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  for (MSG_T i = 0; i <= 2 * DEFAULT_BUFFER; ++i) output.TryPut(0);

  while ( !output.TryPut( 1 ) ) {
    const auto res{ input.GetCurrent() }; // operational!
    REQUIRE( std::unordered_set{0, 1}.count(res) );
  }

  while ( input.GetCurrent() != 1 ) {
    const auto res{ input.GetCurrent() }; // operational!
    REQUIRE( std::unordered_set{0, 1}.count(res) );
  }

  REQUIRE( input.GetCurrent() == 1 );

  uit::verify(MPI_Barrier( MPI_COMM_WORLD )); // todo why

} }

TEST_CASE("Validity") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  int last{};
  for (MSG_T msg = 0; msg < 10 * std::kilo{}.num; ++msg) {

    output.TryPut(msg);

    const MSG_T current = input.GetCurrent();
    REQUIRE( current >= 0 );
    REQUIRE( current < 10 * std::kilo{}.num );
    if ( last > current ) std::cout << msg;
    REQUIRE( last <= current);

    last = current;

  }

  uit::verify( MPI_Barrier(MPI_COMM_WORLD) ); // todo why

} }

TEST_CASE("Ring Mesh connectivity") { REPEAT {

  auto [input, output] = make_ring_bundle();

  // check that everyone's connected properly
  output.TryPut(uit::get_rank());

  REQUIRE( input.GetNext() == uit::numeric_cast<MSG_T>(
    uit::circular_index(uit::get_rank(), uit::get_nprocs(), -1)
  ) );

  uit::verify( MPI_Barrier(MPI_COMM_WORLD) ); // todo why

} }

TEST_CASE("Ring Mesh sequential consistency") { {

  auto [input, output] = make_ring_bundle();

  REQUIRE(input.GetCurrent() == 0);

  // long enough to check that buffer wraparound works properly
  for (MSG_T i = 1; i <= 2 * DEFAULT_BUFFER; ++i) {

    uit::verify( MPI_Barrier( MPI_COMM_WORLD ) );
    output.SurePut(i);
    REQUIRE(input.GetNext() == i);

  }

  uit::verify( MPI_Barrier(MPI_COMM_WORLD) ); // todo why

} }

TEST_CASE("Producer-Consumer Mesh connectivity") { REPEAT {

  auto [input, output] = make_producer_consumer_bundle();

  // check that everyone's connected properly
  if (output) output->SurePut(uit::get_rank());

  // did we get expected rank ID as message?
  if (uit::get_nprocs() % 2 && uit::get_rank() + 1 == uit::get_nprocs()) {
    // is odd process loop at end
    REQUIRE( input->GetNext() == uit::get_rank());
  } else if (input) {
    // is consumer
    REQUIRE(
      input->GetNext() == uit::numeric_cast<int>(
        uit::circular_index(uit::get_rank(), uit::get_nprocs(), -1)
      )
    );
  } else REQUIRE( uit::get_rank() % 2 == 0 ); // is producer

  uit::verify( MPI_Barrier(MPI_COMM_WORLD) ); // todo why

} }

TEST_CASE("Producer-Consumer Mesh sequential consistency") { {

  auto [input, output] = make_producer_consumer_bundle();

  // outlet should be seeded with default-constructed T
  if (input) REQUIRE( input->GetCurrent() == 0 );

  // long enough to check that buffer wraparound works properly
  for (MSG_T i = 1; i <= 2 * DEFAULT_BUFFER; ++i) {

    uit::verify( MPI_Barrier( MPI_COMM_WORLD ) );
    if (output) output->SurePut(i);
    if (input) REQUIRE( input->GetNext() == i );

  }

  uit::verify( MPI_Barrier(MPI_COMM_WORLD) ); // todo why

} }

TEST_CASE("Dyadic Mesh connectivity") { REPEAT {

  auto [input, output] = make_dyadic_bundle();
  uit::verify( MPI_Barrier(MPI_COMM_WORLD) );

  // check that everyone's connected properly
  output.SurePut(uit::get_rank());

  // did we get expected rank ID as message?
  if (uit::get_nprocs() % 2 && uit::get_rank() + 1 == uit::get_nprocs()) {
    // is connected to self (is odd process loop at end)
    REQUIRE( input.GetNext() == uit::get_rank() );
  } else {
    // is connected to neighbor
    REQUIRE( input.GetNext() == uit::numeric_cast<int>(
      uit::circular_index(
        uit::get_rank(),
        uit::get_nprocs(),
        // is pointing forwards or backwards
        (uit::get_rank() % 2) ? -1 : 1
      )
    ) );
  }

  uit::verify( MPI_Barrier(MPI_COMM_WORLD) ); // todo why

} }

TEST_CASE("Dyadic Mesh sequential consistency") { {

  auto [input, output] = make_dyadic_bundle();

  // outlet should be seeded with default-constructed T
  REQUIRE( input.GetCurrent() == 0 );

  // long enough to check that buffer wraparound works properly
  for (MSG_T i = 1; i <= 2 * DEFAULT_BUFFER; ++i) {

    uit::verify( MPI_Barrier( MPI_COMM_WORLD ) );
    output.SurePut(i);
    REQUIRE( input.GetNext() == i );

  }

  uit::verify( MPI_Barrier(MPI_COMM_WORLD) ); // todo why

} }
