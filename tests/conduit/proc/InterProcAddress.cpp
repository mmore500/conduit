#define CATCH_CONFIG_RUNNER

#include "Catch/single_include/catch2/catch.hpp"
#include "distributed/MultiprocessReporter.hpp"

#include "conduit/proc/InterProcAddress.hpp"

TEST_CASE("Test InterProcAddress") {

  // TODO flesh out stub test
  uit::InterProcAddress(0);

}

int main(int argc, char* argv[]) {

  uit::verify(MPI_Init(&argc, &argv));

  Catch::Session session;
  session.configData().reporterName = "multiprocess";
  int result = session.run( argc, argv );

  uit::verify(MPI_Finalize());

  return result;
}
