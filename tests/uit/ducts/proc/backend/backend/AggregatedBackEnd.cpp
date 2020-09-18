#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/base/vector.h"

#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

#include "uit/ducts/proc/backend/AggregatedBackEnd.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=cereal/inlet=RingIsend+outlet=Iprobe_c::IriOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

const uitsl::MpiGuard guard;

template<typename ImplSpec>
using AggregatorDuct = uit::c::IriOiDuct<ImplSpec>;

TEST_CASE("Test AggregatedBackEnd") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;


  uit::AggregatedBackEnd< Spec, AggregatorDuct >{};

}
