#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/Source.hpp"
#include "uit/mesh/MeshNodeInput.hpp"

TEST_CASE("Test MeshNodeInput") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;

  uit::Source<Spec> source;

  uit::MeshNodeInput<Spec>{
    source.GetOutlet(),
    0
  };

}
