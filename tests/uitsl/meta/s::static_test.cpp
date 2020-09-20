#include <set>
#include <string>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"
#include "cereal/include/cereal/types/string.hpp"
#include "cereal/include/cereal/types/vector.hpp"

#include "uitsl/meta/s::static_test.hpp"

TEST_CASE("Test valid types") {

  REQUIRE( uitsl::s::static_test<std::vector<int>>() );

  REQUIRE( uitsl::s::static_test<std::string>() );

}

TEST_CASE("Test invalid types") {

  REQUIRE( !uitsl::s::static_test<int>() );

  REQUIRE( !uitsl::s::static_test<double>() );

  REQUIRE( !uitsl::s::static_test<std::set<double>>() );

}
