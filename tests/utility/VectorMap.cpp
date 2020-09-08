#include "string"

#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "uit/utility/VectorMap.hpp"

TEST_CASE("VectorMap") {

  uit::VectorMap<std::string> map;

  REQUIRE( map[2] == "");
  REQUIRE( map.at(2) == "");

  map[1] = "howdy";
  REQUIRE( map[1] == "howdy" );
  REQUIRE( map[2] == "" );

  map[4] = "cowsay";
  REQUIRE( map.at(4) == "cowsay" );
  REQUIRE( map[4] == "cowsay" );

  REQUIRE( map[1] == "howdy" );
  REQUIRE( map[2] == "" );

}
