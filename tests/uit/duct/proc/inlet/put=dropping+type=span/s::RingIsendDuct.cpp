#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "Empirical/source/base/vector.h"

#include "uit/config/ImplSpec.hpp"
#include "uit/conduit/../config/InterProcAddress.hpp"
#include "uit/duct/proc/inlet/put=dropping+type=span/s::RingIsendDuct.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test s::RingIsendDuct") {

  using ImplSpec = uit::ImplSpec<emp::vector<char>>;
  using BackEnd = uit::s::RingIsendDuct<ImplSpec>::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>() };
  uit::s::RingIsendDuct<ImplSpec>{ address, backing };

}
