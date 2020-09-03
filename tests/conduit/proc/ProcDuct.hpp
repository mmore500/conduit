#include <ratio>
#include <thread>

#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/Sink.hpp"
#include "uit/conduit/Source.hpp"
#include "uit/distributed/assign_utils.hpp"
#include "uit/distributed/MPIGuard.hpp"
#include "uit/distributed/mpi_utils.hpp"
#include "uit/distributed/MultiprocessReporter.hpp"
#include "uit/distributed/RdmaWindowManager.hpp"
#include "uit/mesh/Mesh.hpp"
#include "uit/mesh/MeshNodeInput.hpp"
#include "uit/mesh/MeshNodeOutput.hpp"
#include "uit/topology/DyadicTopologyFactory.hpp"
#include "uit/topology/ProConTopologyFactory.hpp"
#include "uit/topology/RingTopologyFactory.hpp"
#include "uit/utility/CircularIndex.hpp"
#include "uit/utility/math_utils.hpp"

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

// TODO why doesn't this work with openmpi with RingRdmaDuct?
// TEST_CASE("Is initial Get() result value-intialized?") { REPEAT {
//
//   std::shared_ptr<Spec::ProcBackEnd> backend{
//     std::make_shared<Spec::ProcBackEnd>()
//   };
//
//   auto [outlet] = uit::Source<Spec>{
//     std::in_place_type_t<Spec::ProcOutletDuct>{},
//     uit::InterProcAddress{
//       uit::get_rank(),
//       uit::numeric_cast<int>(
//         uit::circular_index(uit::get_rank(), uit::get_nprocs(), 1)
//       )
//     },
//     backend
//   };
//
//   // corresponding sink
//   uit::Sink<Spec>{
//     std::in_place_type_t<Spec::ProcOutletDuct>{},
//     uit::InterProcAddress{
//       uit::get_rank(),
//       uit::numeric_cast<int>(
//         uit::circular_index(uit::get_rank(), uit::get_nprocs(), -1)
//       )
//     },
//     backend
//   };
//
//   backend->Initialize();
//
//   REQUIRE( outlet.Get() == MSG_T{} );
//   REQUIRE( outlet.JumpGet() == MSG_T{} );
//
// } }

TEST_CASE("Is initial Get() result value-intialized?") { REPEAT {

  auto [input, output] = make_ring_bundle();

  REQUIRE( input.Get() == MSG_T{} );
  REQUIRE( input.JumpGet() == MSG_T{} );

} }

TEST_CASE("Unmatched gets") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  for (MSG_T i = 0; i <= 2 * DEFAULT_BUFFER; ++i) {
    REQUIRE( input.JumpGet() == MSG_T{} );
  }

  UIT_Barrier( MPI_COMM_WORLD );

  output.Put(42);
  while( input.JumpGet() != 42);

  for (MSG_T i = 0; i <= 2 * DEFAULT_BUFFER; ++i) {
    REQUIRE( input.JumpGet() == 42 );
  }

} }

TEST_CASE("Unmatched puts") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  for (MSG_T i = 0; i <= 2 * DEFAULT_BUFFER; ++i) output.TryPut(i);

  REQUIRE( input.JumpGet() <= 2 * DEFAULT_BUFFER );

  UIT_Barrier( MPI_COMM_WORLD ); // todo why

} }

TEST_CASE("Eventual flush-out") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  for (MSG_T i = 0; i <= 2 * DEFAULT_BUFFER; ++i) output.TryPut(0);

  while ( !output.TryPut( 1 ) ) {
    const auto res{ input.JumpGet() }; // operational!
    REQUIRE( std::unordered_set{0, 1}.count(res) );
  }

  while ( input.JumpGet() != 1 ) {
    const auto res{ input.JumpGet() }; // operational!
    REQUIRE( std::unordered_set{0, 1}.count(res) );
  }

  REQUIRE( input.JumpGet() == 1 );

  UIT_Barrier( MPI_COMM_WORLD ); // todo why

} }

TEST_CASE("Validity") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  int last{};
  for (MSG_T msg = 0; msg < 10 * std::kilo{}.num; ++msg) {

    output.TryPut(msg);

    const MSG_T current = input.JumpGet();
    REQUIRE( current >= 0 );
    REQUIRE( current < 10 * std::kilo{}.num );
    REQUIRE( last <= current);

    last = current;

  }

  UIT_Barrier(MPI_COMM_WORLD); // todo why

} }
