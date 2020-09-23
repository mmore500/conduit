#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/base/vector.h"

#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"

#include "uit/ducts/proc/impl/outlet/accumulating+type=spanfundamental/sf::WithdrawingWindowDuct.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/setup/InterProcAddress.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test sf::WithdrawingWindowDuct") {

  using ImplSpec = uit::MockSpec<emp::vector<char>>;
  using BackEnd = uit::sf::WithdrawingWindowDuct<ImplSpec>::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>( 42 ) };
  uit::sf::WithdrawingWindowDuct<ImplSpec>{ address, backing };

}
