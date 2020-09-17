#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/setup/ImplSpec.hpp"
#include "uit/ducts/proc/inlet/templated/impl/BufferSpec.hpp"
#include "uit/ducts/proc/put=dropping+get=skipping+type=span/inlet=RingIsend+outlet=BlockIrecv_s::IriObiDuct.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test BufferSpec") {

  // TODO flesh out stub test
  using ImplSpec = uit::ImplSpec<char>;
  uit::internal::BufferSpec<
    ImplSpec,
    uit::s::IriObiDuct
  >{};

}
