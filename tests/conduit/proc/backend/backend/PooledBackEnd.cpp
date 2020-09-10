#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/base/vector.h"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/backend/PooledBackEnd.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping/inlet=VectorRingIsend+outlet=VectorBlockIrecv_IvriOvbiDuct.hpp"
#include "uit/mpi/MpiGuard.hpp"
#include "uit/debug/MultiprocessReporter.hpp"

const uit::MpiGuard guard;

template<typename ImplSpec>
using PoolDuct = uit::IvriOvbiDuct<ImplSpec>;

TEST_CASE("Test PooledBackEnd") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;


  uit::PooledBackEnd< Spec, PoolDuct >{};

}
