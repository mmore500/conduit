#include <ratio>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/utility/CoarseClock.hpp"
#include "uit/countdown/ProgressBar.hpp"
#include "uit/countdown/Timer.hpp"
#include "uit/countdown/Counter.hpp"

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
