#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/config/ImplSpec.hpp"
#include "uit/conduit/../config/InterProcAddress.hpp"
#include "uit/duct/proc/outlet/templated/BufferedOutletDuct.hpp"
#include "uit/duct/proc/put=dropping+get=stepping+type=span/inlet=RingIsend+outlet=Iprobe_s::IriOiDuct.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test BufferedOutletDuct") {

  using ImplSpec = uit::ImplSpec<char>;
  using BackEnd = uit::BufferedOutletDuct<
    uit::s::IriOiDuct,
    ImplSpec
  >::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>() };
  uit::BufferedOutletDuct<
    uit::s::IriOiDuct,
    ImplSpec
  >{ address, backing };

}
