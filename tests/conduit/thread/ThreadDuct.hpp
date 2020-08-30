#include <ratio>
#include <thread>
#include <unordered_set>

#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_RUNNER
#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/Source.hpp"
#include "conduit/ImplSpec.hpp"
#include "concurrent/Gatherer.hpp"
#include "distributed/MPIGuard.hpp"
#include "distributed/mpi_utils.hpp"
#include "distributed/MultiprocessReporter.hpp"
#include "mesh/Mesh.hpp"
#include "parallel/ThreadTeam.hpp"
#include "polyfill/barrier.hpp"
#include "topology/DyadicTopologyFactory.hpp"
#include "topology/ProConTopologyFactory.hpp"
#include "topology/RingTopologyFactory.hpp"
#include "utility/assign_utils.hpp"
#include "utility/benchmark_utils.hpp"
#include "utility/CircularIndex.hpp"
#include "utility/math_utils.hpp"
#include "utility/numeric_cast.hpp"
#include "utility/safe_compare.hpp"

const uit::MPIGuard guard;

using MSG_T = int;
using Spec = uit::ImplSpec<MSG_T, DEFAULT_BUFFER, ImplSel>;

#define REPEAT for (size_t rep = 0; rep < std::deca{}.num ; ++rep)

#define THREADED_BEGIN uit::ThreadTeam team; for (uit::thread_id_t thread_id = 0; thread_id < num_threads; ++thread_id) { team.Add([thread_id, &mesh](){

#define THREADED_END }); } team.Join();

size_t num_threads;

// must be emplacedd
static std::optional<std::barrier<>> barrier;

TEST_CASE("Is initial Get() result value-intialized?") { REPEAT {

  auto [outlet] = uit::Source<Spec>{
    std::in_place_type_t<Spec::ThreadDuct>{}
  };

  REQUIRE( outlet.GetCurrent() == MSG_T{} );

} }

TEST_CASE("Unmatched gets") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::DyadicTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    for (MSG_T i = 0; i <= 2 * DEFAULT_BUFFER; ++i) {
      REQUIRE( input.GetCurrent() == MSG_T{} );
    }

    barrier->arrive_and_wait();

    output.SurePut(42);
    REQUIRE( input.GetNext() == 42 );

    for (MSG_T i = 0; i <= 2 * DEFAULT_BUFFER; ++i) {
      REQUIRE( input.GetCurrent() == 42 );
    }

  } THREADED_END

} }

TEST_CASE("Unmatched puts") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::DyadicTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    for (MSG_T i = 0; i <= 2 * DEFAULT_BUFFER; ++i) output.TryPut(i);

    REQUIRE( input.GetCurrent() <= 2 * DEFAULT_BUFFER );

  } THREADED_END

} }

TEST_CASE("Eventual flush-out") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::DyadicTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

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

  } THREADED_END

} }


TEST_CASE("Validity") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::DyadicTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    int last{};
    for (MSG_T msg = 0; msg < 10 * std::kilo{}.num; ++msg) {

      output.TryPut(msg);

      const MSG_T current = input.GetCurrent();
      REQUIRE( current >= 0 );
      REQUIRE( current < 10 * std::kilo{}.num );
      REQUIRE( last <= current);

      last = current;

    }

  } THREADED_END

} }

TEST_CASE("Ring Mesh connectivity") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    // check that everyone's connected properly
    output.TryPut(uit::get_rank());

    REQUIRE( input.GetNext() == uit::numeric_cast<MSG_T>(
      uit::circular_index(uit::get_rank(), uit::get_nprocs(), -1)
    ) );

  } THREADED_END

} }

TEST_CASE("Ring Mesh sequential consistency") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    // long enough to check that buffer wraparound works properly
    for (MSG_T i = 1; i <= 2 * DEFAULT_BUFFER; ++i) {

      barrier->arrive_and_wait();

      output.SurePut(i);
      REQUIRE(input.GetNext() == i);

    }

  } THREADED_END

} }

TEST_CASE("Producer-Consumer Mesh connectivity") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInputOrNullopt(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutputOrNullopt(0);

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

  } THREADED_END

} }

TEST_CASE("Producer-Consumer Mesh sequential consistency") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInputOrNullopt(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutputOrNullopt(0);

    // long enough to check that buffer wraparound works properly
    for (MSG_T i = 1; i <= 2 * DEFAULT_BUFFER; ++i) {

      if (output) output->SurePut(i);
      if (input) REQUIRE( input->GetNext() == i );

    }

  } THREADED_END

} }

TEST_CASE("Dyadic Mesh connectivity") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::DyadicTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

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

  } THREADED_END

} }

TEST_CASE("Dyadic Mesh sequential consistency") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::DyadicTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    // long enough to check that buffer wraparound works properly
    for (MSG_T i = 1; i <= 2 * DEFAULT_BUFFER; ++i) {

      barrier->arrive_and_wait();

      output.SurePut(i);
      REQUIRE( input.GetNext() == i );

    }

  } THREADED_END

} }

int main( int argc, char* argv[] ) {

  Catch::Session session; // There must be exactly one instance

  // Build a new parser on top of Catch's
  using namespace Catch::clara;
  session.cli(
    // Get Catch's composite command line parser
    session.cli()
    // bind variable to a new option, with a hint string
    | Opt( num_threads, "num_threads" )
      // the option names it will respond to
      ["-t"]["--num_threads"]
      // description string for the help output
      ("how many threds to test with?")
  );

  // Let Catch (using Clara) parse the command line
  int returnCode = session.applyCommandLine( argc, argv );
  if( returnCode != 0 ) return returnCode;

  barrier.emplace( uit::numeric_cast<std::ptrdiff_t>(num_threads) );

  return session.run();
}
