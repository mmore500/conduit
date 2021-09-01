#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/NotImplementedException.hpp"

TEST_CASE("NotImplementedException", "[nproc:1]") {

  uitsl::NotImplementedException{};

}
