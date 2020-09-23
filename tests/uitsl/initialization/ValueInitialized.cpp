#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/utility/ValueInitialized.hpp"

TEST_CASE("Test ValueInitialized") {

  uitsl::ValueInitialized<int> val1;
  REQUIRE( val1 == 0 );
  REQUIRE( *(&val1) == 0 );
  val1 = 10;
  REQUIRE( val1 == 10 );
  REQUIRE( *(&val1) == 10 );

  uitsl::ValueInitialized<double> val2;
  REQUIRE( val2 == 0.0 );
  REQUIRE( *(&val2) == 0.0 );
  val2 = -10.0;
  REQUIRE( val2 == -10.0 );
  REQUIRE( *(&val2) == -10.0 );

}
