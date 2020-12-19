#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/compare_files.hpp"
#include "uitsl/fetch/fetch.hpp"

TEST_CASE("fetch", "[nproc:1]") {

  REQUIRE( uitsl::compare_files(
    "assets/TheWorldWideWebProject.html",
    uitsl::fetch( "http://info.cern.ch/hypertext/WWW/TheProject.html" )
  ) );

}
