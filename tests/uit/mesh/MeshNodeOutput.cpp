#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/setup/ImplSpec.hpp"
#include "uit/fixtures/Sink.hpp"
#include "uit/mesh/MeshNodeOutput.hpp"

TEST_CASE("Test MeshNodeOutput") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;

  uit::Sink<Spec> sink;

  // TODO flesh out stub test
  uit::MeshNodeOutput<Spec>{
    sink.GetInlet(),
    0
  };

}
