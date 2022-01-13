#include <type_traits>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/polyfill/remove_cvref.hpp"

TEST_CASE("Test remove_cvref", "[nproc:1]") {

  // adapted from https://en.cppreference.com/w/cpp/types/remove_cvref
  REQUIRE( std::is_same_v<std::remove_cvref_t<int>, int> );
  REQUIRE( std::is_same_v<std::remove_cvref_t<int&>, int> );
  REQUIRE( std::is_same_v<std::remove_cvref_t<int&&>, int> );
  REQUIRE( std::is_same_v<std::remove_cvref_t<const int&>, int> );
  REQUIRE( std::is_same_v<std::remove_cvref_t<const int[2]>, int[2]> );
  REQUIRE( std::is_same_v<std::remove_cvref_t<const int(&)[2]>, int[2]> );
  REQUIRE( std::is_same_v<std::remove_cvref_t<int(int)>, int(int)> );

  REQUIRE( !std::is_same_v<std::remove_cvref_t<int>, double> );
  REQUIRE( !std::is_same_v<std::remove_cvref_t<int&>, int[2]> );
  REQUIRE( !std::is_same_v<std::remove_cvref_t<int&&>, int*> );
  REQUIRE( !std::is_same_v<std::remove_cvref_t<const int&>, double*> );
  REQUIRE( !std::is_same_v<std::remove_cvref_t<const int[2]>, int&> );
  REQUIRE( !std::is_same_v<std::remove_cvref_t<const int(&)[2]>, double[2]> );

}
