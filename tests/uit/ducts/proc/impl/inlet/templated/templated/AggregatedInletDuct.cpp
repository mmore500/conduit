#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/ducts/proc/impl/inlet/templated/AggregatedInletDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=cereal/inlet=RingIsend+outlet=Iprobe_c::IriOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/setup/InterProcAddress.hpp"

TEST_CASE("Test AggregatedInletDuct") {

  using ImplSpec = uit::MockSpec<char>;
  using BackEnd = uit::AggregatedInletDuct<
    uit::c::IriOiDuct,
    ImplSpec
  >::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>() };
  uit::AggregatedInletDuct<
    uit::c::IriOiDuct,
    ImplSpec
  >{ address, backing };

}
