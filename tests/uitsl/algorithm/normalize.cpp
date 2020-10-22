#include <list>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/algorithm/normalize.hpp"

TEST_CASE("Test normalize") {

  std::list<double> target{ 0.0, 2.0, 2.5, 0.5 };

  uitsl::normalize( std::begin( target ), std::end( target ) );

  REQUIRE(( target == std::list<double>{0.0, 0.4, 0.5, 0.1} ));

}

TEST_CASE("Test normalize zero length") {

  std::vector<double> target{ };

  uitsl::normalize( std::begin( target ), std::end( target ) );

  REQUIRE( target.size() == 0 );

};

TEST_CASE("Test normalize zero sum") {

  std::vector<double> target{ 0.0, 0.0, 0.0 };

  uitsl::normalize( std::begin( target ), std::end( target ) );

  REQUIRE(( target == std::vector<double>{0.0, 0.0, 0.0} ));

};
