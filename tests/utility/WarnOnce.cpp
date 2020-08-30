#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "uit/utility/WarnOnce.hpp"

TEST_CASE("WarnOnce") {

  static const uit::WarnOnce warning{"howdy"};

}
