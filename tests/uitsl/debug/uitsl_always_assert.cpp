#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/uitsl_always_assert.hpp"

TEST_CASE("uitsl_always_assert", "[nproc:1]") {

  // uitsl_always_assert(false);

  // uitsl_always_assert(true);
  //
  // uitsl_always_assert(true, );

  uitsl_always_assert(true, "");

  const int x = 10;
  uitsl_always_assert(x == 10, x << "text" << x*x);

  // uitsl_always_assert(x == 12, x << "text" << x*x);

}
