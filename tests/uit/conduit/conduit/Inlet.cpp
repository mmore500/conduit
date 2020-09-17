#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/Duct.hpp"
#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/Inlet.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test Inlet") {

  using Spec = uit::ImplSpec<char>;
  uit::Inlet<Spec>{
    std::make_shared<uit::internal::Duct<Spec>>()
  };

}
