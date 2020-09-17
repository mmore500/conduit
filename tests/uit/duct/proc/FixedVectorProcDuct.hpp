#include <ratio>
#include <set>
#include <thread>

#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "Empirical/source/base/vector.h"

#include "uit/config/ImplSpec.hpp"
#include "uit/conduit/Sink.hpp"
#include "uit/conduit/Source.hpp"
#include "uitsl/distributed/assign_utils.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/mpi/mpi_utils.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/distributed/RdmaWindowManager.hpp"
#include "uit/mesh/Mesh.hpp"
#include "uit/mesh/MeshNodeInput.hpp"
#include "uit/mesh/MeshNodeOutput.hpp"
#include "uit/topology/DyadicTopologyFactory.hpp"
#include "uit/topology/ProConTopologyFactory.hpp"
#include "uit/topology/RingTopologyFactory.hpp"
#include "uitsl/nonce/CircularIndex.hpp"
#include "uitsl/math/math_utils.hpp"

const uitsl::MpiGuard guard;

using MSG_T = emp::vector<int>;
using Spec = uit::ImplSpec<MSG_T, uit::DEFAULT_BUFFER, ImplSel>;

#define REPEAT for (size_t rep = 0; rep < std::deca{}.num; ++rep)

int tag{};

TEST_CASE("Is initial Get() result value-intialized?") { REPEAT {

  std::shared_ptr<Spec::ProcBackEnd> backend{
    std::make_shared<Spec::ProcBackEnd>(2)
  };

  auto [outlet] = uit::Source<Spec>{
    std::in_place_type_t<Spec::ProcOutletDuct>{},
    uit::InterProcAddress{
      uitsl::get_rank(), // outlet proc
      uitsl::safe_cast<int>(
        uitsl::circular_index(uitsl::get_rank(), uitsl::get_nprocs(), -1)
      ), // inlet proc
      0, // outlet thread
      0, // inlet thread
      tag // tag
    },
    backend
  };

  // corresponding sink
  uit::Sink<Spec>{
    std::in_place_type_t<Spec::ProcInletDuct>{},
    uit::InterProcAddress{
      uitsl::safe_cast<int>(
        uitsl::circular_index(uitsl::get_rank(), uitsl::get_nprocs(), 1)
      ), // outlet proc
      uitsl::get_rank(), // inlet proc
      0, // outlet thread
      0, // inlet thread
      tag++ // tag
    },
    backend
  };

  backend->Initialize();

  REQUIRE( outlet.Get() == MSG_T{0, 0} );
  REQUIRE( outlet.JumpGet() == MSG_T{0, 0} );

} }

TEST_CASE("Is transmission reliable?") { REPEAT {

  std::shared_ptr<Spec::ProcBackEnd> backend{
    std::make_shared<Spec::ProcBackEnd>(1)
  };

  auto [outlet] = uit::Source<Spec>{
    std::in_place_type_t<Spec::ProcOutletDuct>{},
    uit::InterProcAddress{
      uitsl::get_rank(), // outlet proc
      uitsl::safe_cast<int>(
        uitsl::circular_index(uitsl::get_rank(), uitsl::get_nprocs(), -1)
      ), // inlet proc
      0, // outlet thread
      0, // inlet thread
      tag // tag
    },
    backend
  };

  // corresponding sink
  auto [inlet] = uit::Sink<Spec>{
    std::in_place_type_t<Spec::ProcInletDuct>{},
    uit::InterProcAddress{
      uitsl::safe_cast<int>(
        uitsl::circular_index(uitsl::get_rank(), uitsl::get_nprocs(), 1)
      ), // outlet proc
      uitsl::get_rank(), // inlet proc
      0,
      0,
      tag++ // tag
    },
    backend
  };

  backend->Initialize();

  inlet.Put( {10} );
  inlet.Put( {20} );
  inlet.Put( {30} );

  for (int i = 0; i < 1000; ++i) {
    outlet.JumpGet();
  }

  while (  outlet.JumpGet() != MSG_T{30} ) REQUIRE(
    std::set<MSG_T>{{0}, {10}, {20}}.count(outlet.Get())
  );

  REQUIRE( outlet.Get() == MSG_T{30} );

  UIT_Barrier( MPI_COMM_WORLD );

} }
