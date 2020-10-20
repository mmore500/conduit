#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#define __EMSCRIPTEN__
#include "uitsl/polyfill/filesystem.hpp"

TEST_CASE("Test filesystem") {

  REQUIRE( std::filesystem::path{ "test.txt" }.extension() == ".txt" );

}
