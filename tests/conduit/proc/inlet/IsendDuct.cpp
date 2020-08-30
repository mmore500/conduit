#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/InterProcAddress.hpp"
#include "uit/conduit/proc/inlet/IsendDuct.hpp"
#include "uit/distributed/MPIGuard.hpp"
#include "uit/distributed/MultiprocessReporter.hpp"

const uit::MPIGuard guard;

TEST_CASE("Test IsendDuct") {

  using ImplSpec = uit::ImplSpec<char>;
  using BackEnd = uit::IsendDuct<ImplSpec>::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>() };
  uit::IsendDuct<ImplSpec>{ address, backing };

}
