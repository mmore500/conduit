#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "conduit/ImplSpec.hpp"


TEST_CASE("Test ImplSelector") {

  uit::ImplSelector<>{};

}

TEST_CASE("Test ImplSpec") {

  uit::ImplSpec<char>{};

}
