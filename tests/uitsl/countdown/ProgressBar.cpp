#include <ratio>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/chrono/CoarseClock.hpp"
#include "uitsl/countdown/ProgressBar.hpp"
#include "uitsl/countdown/Timer.hpp"
#include "uitsl/countdown/Counter.hpp"

TEST_CASE("Test CountdownProgresBar<Counter>") {

  using bar_t = uit::ProgressBar<uit::Counter>;

  for ( [[maybe_unused]] auto it : bar_t{ std::mega{}.num })
    ;

}

TEST_CASE("Test CountdownProgresBar<Timer>") {

  using timer_t = uit::Timer<
    std::chrono::duration<double, std::ratio<1>>,
    uit::CoarseClock
  >;
  using bar_t = uit::ProgressBar<timer_t>;

  for ( [[maybe_unused]] auto it : bar_t{ std::chrono::seconds{3} })
    ;

}
