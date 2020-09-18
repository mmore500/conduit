#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"

#include "uit/ducts/proc/outlet/accumulating+type=fundamental/f::WithdrawingWindowDuct.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/setup/InterProcAddress.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test f::WithdrawingWindowDuct") {

  using ImplSpec = uit::ImplSpec<char>;
  using BackEnd = uit::f::WithdrawingWindowDuct<ImplSpec>::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>() };
  uit::f::WithdrawingWindowDuct<ImplSpec>{ address, backing };

}
