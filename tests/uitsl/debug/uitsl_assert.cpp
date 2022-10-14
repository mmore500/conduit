#define NDEBUG

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/uitsl_assert.hpp"

TEST_CASE("uitsl_assert", "[nproc:1]") {


  // uitsl_assert(true);
  //
  // uitsl_assert(true, );

  uitsl_assert(true, "");

  const int x = 10;
  uitsl_assert(x == 12, x << "text" << x*x);

}
