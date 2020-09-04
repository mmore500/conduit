#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/InterProcAddress.hpp"
#include "uit/conduit/proc/inlet/put=growing/DequeIsendDuct.hpp"
#include "uit/mpi/MPIGuard.hpp"
#include "uit/debug/MultiprocessReporter.hpp"

const uit::MPIGuard guard;

TEST_CASE("Test DequeIsendDuct") {

  using ImplSpec = uit::ImplSpec<char>;
  using BackEnd = uit::DequeIsendDuct<ImplSpec>::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>() };
  uit::DequeIsendDuct<ImplSpec>{ address, backing };

}
