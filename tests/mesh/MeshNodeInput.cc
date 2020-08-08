#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/ImplSpec.h"
#include "conduit/Source.h"
#include "mesh/MeshNodeInput.h"

TEST_CASE("Test MeshNodeInput") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;

  uit::Source<Spec> source;

  uit::MeshNodeInput<Spec>{
    source.GetOutlet(),
    0
  };

}
