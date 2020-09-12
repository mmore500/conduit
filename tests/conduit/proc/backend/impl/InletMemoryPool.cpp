#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/backend/impl/InletMemoryPool.hpp"
#include "uit/conduit/proc/backend/impl/PoolSpec.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping+type=span/inlet=RingIsend+outlet=BlockIrecv_s::IriObiDuct.hpp"
#include "uit/mpi/MpiGuard.hpp"
#include "uit/debug/MultiprocessReporter.hpp"

const uit::MpiGuard guard;

TEST_CASE("Test InletMemoryPool") {

  // TODO flesh out stub test
  using PoolSpec_t = uit::PoolSpec<
    uit::ImplSpec<char>,
    uit::s::IriObiDuct
  >;
  uit::InletMemoryPool<PoolSpec_t>{};

}
