#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/ImplSpec.hpp"
#include "conduit/Source.hpp"
#include "mesh/MeshNodeInput.hpp"

TEST_CASE("Test MeshNodeInput") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;

  uit::Source<Spec> source;

  uit::MeshNodeInput<Spec>{
    source.GetOutlet(),
    0
  };

}
