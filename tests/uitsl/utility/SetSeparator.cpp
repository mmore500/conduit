#include <sstream>

#include "Catch/single_include/catch2/catch.hpp"
#include "stduuid/include/uuid.h"

#include "uitsl/utility/SetSeparator.hpp"

TEST_CASE("SetSeparator no items", "[nproc:1]") {
  std::stringstream ss;
  ss << uitsl::SetSeparator(", ");
  REQUIRE(ss.str() == "");
}

TEST_CASE("SetSeparator one item", "[nproc:1]") {
  std::stringstream ss;
  ss << uitsl::SetSeparator(", ") << "hello";
  REQUIRE(ss.str() == "hello");
}

TEST_CASE("SetSeparator two items", "[nproc:1]") {
  std::stringstream ss;
  ss << uitsl::SetSeparator(", ") << "hello" << "friend";
  REQUIRE(ss.str() == "hello, friend");
}
