#include <array>
#include <string>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"
#include "cereal/include/cereal/types/string.hpp"
#include "cereal/include/cereal/types/vector.hpp"

#include "uitsl/meta/f::static_test.hpp"

TEST_CASE("Test valid types") {

  REQUIRE( uitsl::f::static_test<int>() );

  REQUIRE( uitsl::f::static_test<double>() );

}

TEST_CASE("Test invalid types") {

  REQUIRE( !uitsl::f::static_test<std::vector<int>>() );

  REQUIRE( !uitsl::f::static_test<std::array<double, 3>>() );

  REQUIRE( !uitsl::f::static_test<std::string>() );

}
