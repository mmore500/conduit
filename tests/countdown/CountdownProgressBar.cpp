#include <ratio>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/countdown/CoarseClock.hpp"
#include "uit/countdown/CountdownProgressBar.hpp"
#include "uit/countdown/CountdownTimer.hpp"
#include "uit/countdown/CountdownIterator.hpp"

TEST_CASE("Test CountdownProgresBar<CountdownIterator>") {

  using bar_t = uit::CountdownProgressBar<uit::CountdownIterator>;

  for ( [[maybe_unused]] auto it : bar_t{ std::mega{}.num })
    ;

}

TEST_CASE("Test CountdownProgresBar<CountdownTimer>") {

  using timer_t = uit::CountdownTimer<
    std::chrono::duration<double, std::ratio<1>>,
    uit::CoarseClock
  >;
  using bar_t = uit::CountdownProgressBar<timer_t>;

  for ( [[maybe_unused]] auto it : bar_t{ std::chrono::seconds{3} })
    ;

}
