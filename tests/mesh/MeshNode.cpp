#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/Conduit.hpp"
#include "conduit/ImplSpec.hpp"
#include "mesh/MeshNode.hpp"

TEST_CASE("Test MeshNode") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;
  uit::Conduit<Spec> conduit;

  auto & [inlet, outlet] = conduit;

  // TODO flesh out stub test
  uit::MeshNode<Spec> node{0};

  node.AddInput(uit::MeshNodeInput<Spec>{outlet, 0});
  node.AddOutput(uit::MeshNodeOutput<Spec>{inlet, 0});

}
