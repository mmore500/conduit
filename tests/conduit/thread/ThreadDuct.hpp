#include <ratio>
#include <thread>
#include <unordered_set>

#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_RUNNER
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/base/optional.h"

#include "uit/conduit/Source.hpp"
#include "uit/conduit/ImplSpec.hpp"
#include "uit/concurrent/Gatherer.hpp"
#include "uit/mpi/MpiGuard.hpp"
#include "uit/mpi/mpi_utils.hpp"
#include "uit/debug/MultiprocessReporter.hpp"
#include "uit/mesh/Mesh.hpp"
#include "uit/parallel/ThreadTeam.hpp"
#include "uit/polyfill/barrier.hpp"
#include "uit/topology/DyadicTopologyFactory.hpp"
#include "uit/topology/ProConTopologyFactory.hpp"
#include "uit/topology/RingTopologyFactory.hpp"
#include "uit/utility/assign_utils.hpp"
#include "uit/debug/benchmark_utils.hpp"
#include "uit/utility/CircularIndex.hpp"
#include "uit/math/math_utils.hpp"
#include "uit/debug/safe_cast.hpp"
#include "uit/debug/safe_compare.hpp"

const uit::MpiGuard guard;

using MSG_T = int;
using Spec = uit::ImplSpec<MSG_T, DEFAULT_BUFFER, ImplSel>;

#define REPEAT for (size_t rep = 0; rep < std::deca{}.num ; ++rep)

#define THREADED_BEGIN uit::ThreadTeam team; for (uit::thread_id_t thread_id = 0; thread_id < num_threads; ++thread_id) { team.Add([thread_id, &mesh](){

#define THREADED_END }); } team.Join();

size_t num_threads;

// must be emplacedd
static emp::optional<std::barrier<>> barrier;

TEST_CASE("Is initial Get() result value-intialized?") { REPEAT {

  auto [outlet] = uit::Source<Spec>{
    std::in_place_type_t<Spec::ThreadDuct>{}
  };

  REQUIRE( outlet.JumpGet() == MSG_T{} );

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
      REQUIRE( input.JumpGet() == MSG_T{} );
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

    REQUIRE( input.JumpGet() <= (2 * DEFAULT_BUFFER) * (2 * DEFAULT_BUFFER) );

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

  barrier.emplace( uit::safe_cast<std::ptrdiff_t>(num_threads) );

  return session.run();
}
