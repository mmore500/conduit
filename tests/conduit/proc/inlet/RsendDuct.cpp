#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/ImplSpec.hpp"
#include "conduit/InterProcAddress.hpp"
#include "conduit/proc/backend/MockBackEnd.hpp"
#include "conduit/proc/inlet/RsendDuct.hpp"
#include "distributed/MPIGuard.hpp"
#include "distributed/MultiprocessReporter.hpp"

const uit::MPIGuard guard;

TEST_CASE("Test RsendDuct") {

  using ImplSpec = uit::ImplSpec<char>;
  using BackEnd = uit::RsendDuct<ImplSpec>::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>() };
  uit::RsendDuct<ImplSpec>{ address, backing };

}
