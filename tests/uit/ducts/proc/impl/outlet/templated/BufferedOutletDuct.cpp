#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/ducts/proc/impl/outlet/templated/BufferedOutletDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=span/inlet=RingIsend+outlet=Iprobe_s::IriOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/setup/InterProcAddress.hpp"

TEST_CASE("Test BufferedOutletDuct") {

  using ImplSpec = uit::MockSpec<char>;
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
