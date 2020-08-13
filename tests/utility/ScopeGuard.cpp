#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "utility/ScopeGuard.hpp"

TEST_CASE("Test ScopeGuard with default OnDestruct")
{

  size_t i{};
  REQUIRE( i == 0 );

  {
  const uit::ScopeGuard guard{ [&](){++i;} };
  REQUIRE( i == 1 );
  }

  REQUIRE( i == 1 );


}

TEST_CASE("Test ScopeGuard with custom OnDestruct")
{

  size_t i{};
  REQUIRE( i == 0 );

  {
  const uit::ScopeGuard guard{ [&](){++i;}, [&](){--i;} };
  REQUIRE( i == 1 );
  }

  REQUIRE( i == 0 );


}
