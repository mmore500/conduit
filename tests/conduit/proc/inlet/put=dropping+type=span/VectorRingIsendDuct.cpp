#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "Empirical/source/base/vector.h"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/InterProcAddress.hpp"
#include "uit/conduit/proc/inlet/put=dropping+type=span/VectorRingIsendDuct.hpp"
#include "uit/mpi/MpiGuard.hpp"
#include "uit/debug/MultiprocessReporter.hpp"

const uit::MpiGuard guard;

TEST_CASE("Test VectorRingIsendDuct") {

  using ImplSpec = uit::ImplSpec<emp::vector<char>>;
  using BackEnd = uit::VectorRingIsendDuct<ImplSpec>::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>() };
  uit::VectorRingIsendDuct<ImplSpec>{ address, backing };

}
