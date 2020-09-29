#include <set>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/algorithm/get_plurality.hpp"

TEST_CASE("Test upper_uniquify")
{

  std::multiset<int> target{ 1, 1, 2, 5, 6, 9 ,1, 2, 2, 2 };

  REQUIRE( 2 == uitsl::get_plurality( std::begin(target), std::end(target) ) );

}
