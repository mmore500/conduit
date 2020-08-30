#include <stddef.h>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/parallel/ThreadMap.hpp"
#include "uit/parallel/ThreadTeam.hpp"

TEST_CASE("ThreadMap") {

  // TODO expand stub test
  uit::ThreadMap<size_t> map;

  uit::ThreadTeam team;

  for (size_t thread = 0; thread < 4; ++thread) {
    team.Add([&map](){
      REQUIRE(++map.GetWithDefault() == 1);
      REQUIRE(++map.GetWithDefault() == 2);
    });
  }

  team.Join();

  REQUIRE(++map.GetWithDefault() == 1);
  REQUIRE(++map.GetWithDefault() == 2);
  REQUIRE(map.GetSize() == 5);

}
