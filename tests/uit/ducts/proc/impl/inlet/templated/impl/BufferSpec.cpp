#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

#include "uit/ducts/proc/impl/inlet/templated/impl/BufferSpec.hpp"
#include "uit/ducts/proc/put=dropping+get=skipping+type=span/inlet=RingIsend+outlet=BlockIrecv_s::IriObiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test BufferSpec") {

  // TODO flesh out stub test
  using ImplSpec = uit::MockSpec<char>;
  uit::internal::BufferSpec<
    ImplSpec,
    uit::s::IriObiDuct
  >{};

}
