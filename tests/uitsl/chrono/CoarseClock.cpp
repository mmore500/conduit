#include <thread>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/chrono/CoarseClock.hpp"
#include "uitsl/chrono/CoarseRealClock.hpp"
#include "uitsl/countdown/ProgressBar.hpp"
#include "uitsl/countdown/Timer.hpp"

using mytimer_t = uitsl::Timer<
  std::chrono::duration<double, std::ratio<1>>,
  uitsl::CoarseClock
>;
using bar_t = uitsl::ProgressBar<mytimer_t>;

TEST_CASE("Test CoarseClock")
{

  const auto before = uitsl::CoarseClock::now();

  std::this_thread::sleep_for(
    std::chrono::seconds{ 2 }
  );

  const auto after = uitsl::CoarseClock::now();

  REQUIRE( after - before > std::chrono::seconds{ 1 } );
  REQUIRE( after - before < std::chrono::seconds{ 3 } );

}
