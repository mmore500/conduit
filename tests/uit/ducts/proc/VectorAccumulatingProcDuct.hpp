#include <ratio>
#include <thread>

#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/base/vector.h"

#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/distributed/assign_utils.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

#include "netuit/mesh/Mesh.hpp"
#include "netuit/mesh/MeshNodeInput.hpp"
#include "netuit/mesh/MeshNodeOutput.hpp"
#include "netuit/topology/DyadicTopologyFactory.hpp"
#include "netuit/topology/ProConTopologyFactory.hpp"
#include "netuit/topology/RingTopologyFactory.hpp"

#define REPEAT for (size_t rep = 0; rep < std::deca{}.num; ++rep)


using MSG_T = emp::vector<MSG_VALUE_T>;
constexpr size_t message_size = 3;
using Spec = uit::ImplSpec<MSG_T, ImplSel>;

const uitsl::MpiGuard guard;

decltype(auto) make_ring_bundle() {
  netuit::Mesh<Spec> mesh{
    netuit::RingTopologyFactory{}(uitsl::get_nprocs()),
    uitsl::AssignIntegrated<uitsl::thread_id_t>{},
    uitsl::AssignAvailableProcs{},
    std::make_shared<typename Spec::ProcBackEnd>( message_size )
  };

  auto bundles = mesh.GetSubmesh();

  REQUIRE( bundles.size() == 1);

  return std::tuple{ bundles[0].GetInput(0), bundles[0].GetOutput(0) };

}

TEST_CASE("Is initial Get() result value-intialized?") { REPEAT {

  auto [input, output] = make_ring_bundle();

  REQUIRE( input.Get() == MSG_T(message_size) );
  REQUIRE( input.JumpGet() == MSG_T(message_size) );

  UITSL_Barrier( MPI_COMM_WORLD );

} }

TEST_CASE("Unmatched gets") { REPEAT {

  auto [input, output] = make_ring_bundle();

  for (int i = 0; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) {
    REQUIRE( input.JumpGet() == MSG_T(message_size) );
  }

  UITSL_Barrier( MPI_COMM_WORLD );

  output.Put( MSG_T{24, 42, 101} );
  while( output.TryFlush() );
  while( input.JumpGet() != MSG_T{24, 42, 101} );

  REQUIRE( input.Get() == MSG_T{24, 42, 101} );

  REQUIRE( input.JumpGet() == MSG_T(message_size) );

  UITSL_Barrier( MPI_COMM_WORLD );

} }

TEST_CASE("Unmatched puts") { REPEAT {

  auto [input, output] = make_ring_bundle();

  for (int i = 0; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) {
    output.TryPut( MSG_T{1, 2, 3} );
  }

  for (int i = 0; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) {
    output.TryPut( MSG_T{1, 2, 3} );
    output.TryFlush();
  }

  UITSL_Barrier( MPI_COMM_WORLD ); // todo why

} }

TEST_CASE("Validity") { REPEAT {

  auto [input, output] = make_ring_bundle();

  MSG_T sum(message_size);
  // 1/2 n * (n + 1)
  const int expected_sum = (std::kilo{}.num - 1) * std::kilo{}.num / 2;
  for (int msg = 0; msg < std::kilo{}.num; ++msg) {

    output.TryPut(MSG_T(message_size, msg));
    output.TryFlush();

    const MSG_T received = input.JumpGet();
    REQUIRE( received.size() == message_size );
    for (const auto val : received) REQUIRE( val <= expected_sum );
    for (const auto val : received) REQUIRE( val >= 0 );
    std::transform(
      std::begin(received),
      std::end(received),
      std::begin(sum),
      std::begin(sum),
      std::plus{}
    );

  }

  while (sum.front() != expected_sum) {
    const MSG_T received = input.JumpGet();
    REQUIRE( received.size() == message_size );
    for (const auto val : received) REQUIRE( val <= expected_sum );
    for (const auto val : received) REQUIRE( val >= 0 );
    std::transform(
      std::begin(received),
      std::end(received),
      std::begin(sum),
      std::begin(sum),
      std::plus{}
    );
    output.TryFlush();
  }

  while ( !output.TryFlush() );

  REQUIRE( sum.front() == expected_sum );

  for (size_t i = 0; i < 10 * std::kilo{}.num; ++i) {
    REQUIRE( input.JumpGet() == MSG_T(message_size) );
  }

  UITSL_Barrier(MPI_COMM_WORLD); // todo why

} }
