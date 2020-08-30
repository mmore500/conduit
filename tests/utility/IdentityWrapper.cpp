#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "uit/utility/IdentityWrapper.hpp"

TEST_CASE("IdentityWrapper") {

  uit::IdentityWrapper<std::string> wrapped{"present"};
  REQUIRE(wrapped == "present");
  REQUIRE(wrapped.size() == 7);

}
