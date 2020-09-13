#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/InterProcAddress.hpp"
#include "uit/conduit/proc/inlet/templated/BufferedInletDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping+type=span/inlet=RingIsend+outlet=Iprobe_s::IriOiDuct.hpp"
#include "uit/mpi/MpiGuard.hpp"
#include "uit/debug/MultiprocessReporter.hpp"

const uit::MpiGuard guard;

TEST_CASE("Test BufferedInletDuct") {

  using ImplSpec = uit::ImplSpec<char>;
  using BackEnd = uit::BufferedInletDuct<
    uit::s::IriOiDuct,
    ImplSpec
  >::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>() };
  uit::BufferedInletDuct<
    uit::s::IriOiDuct,
    ImplSpec
  >{ address, backing };

}
