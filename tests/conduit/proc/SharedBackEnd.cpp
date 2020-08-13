#define CATCH_CONFIG_RUNNER

#include "Catch/single_include/catch2/catch.hpp"
#include "../../MultiprocessReporter.hpp"

#include "conduit/proc/SharedBackEnd.hpp"
#include "conduit/ImplSpec.hpp"

TEST_CASE("Test SharedBackEnd") {

  // TODO flesh out stub test
  uit::SharedBackEnd<uit::ImplSpec<char>>{};

}

int main(int argc, char* argv[]) {

  uit::verify(MPI_Init(&argc, &argv));

  Catch::Session session;
  session.configData().reporterName = "multiprocess";
  int result = session.run( argc, argv );

  uit::verify(MPI_Finalize());

  return result;
}
