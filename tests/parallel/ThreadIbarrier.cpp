#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/parallel/ThreadIbarrier.hpp"
#include "uit/parallel/ThreadIbarrierFactory.hpp"
#include "uit/parallel/ThreadTeam.hpp"

TEST_CASE("ThreadIbarrier satisfied serial") {

  uit::ThreadIbarrierFactory factory{1};

  uit::ThreadIbarrier first{ factory.MakeBarrier() };
  uit::ThreadIbarrier second{ factory.MakeBarrier() };
  REQUIRE(first.IsComplete());
  REQUIRE(second.IsComplete());

}

TEST_CASE("ThreadIbarrier unsatisfied") {

  uit::ThreadIbarrierFactory factory{2};
  uit::ThreadIbarrier first{ factory.MakeBarrier() };
  uit::ThreadIbarrier second{ factory.MakeBarrier() };
  REQUIRE(!first.IsComplete());
  REQUIRE(!second.IsComplete());

}

TEST_CASE("ThreadIbarrier satisfied parallel") {

  for (size_t thread_count = 1; thread_count <= 16; ++thread_count) {
    std::cout << "thread_count: " << thread_count << std::endl;
    uit::ThreadIbarrierFactory factory{thread_count};
    uit::ThreadTeam team;
    for (size_t thread = 0; thread < thread_count; ++thread) {
      team.Add([&factory](){
        for (size_t rep = 0; rep < 100; ++rep) {
          uit::ThreadIbarrier first{ factory.MakeBarrier() };
          uit::ThreadIbarrier second{ factory.MakeBarrier() };
          while(!first.IsComplete());
          REQUIRE( first.IsComplete() );
          while(!second.IsComplete());
          REQUIRE( second.IsComplete() );
        }
      });
    }
    team.Join();
  }
}
