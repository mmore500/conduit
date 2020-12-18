#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/compare_files.hpp"
#include "uitsl/fetch/inflate.hpp"

TEST_CASE("inflate empty.tar.gz") {

  const auto temp_path{ uitsl::inflate( "assets/empty.tar.gz" ) };

  REQUIRE( uitsl::compare_files( "assets/empty.tar", temp_path ) );

}

TEST_CASE("inflate jabberwocky.tar.gz") {

  const auto temp_path{ uitsl::inflate( "assets/jabberwocky.tar.gz" ) };

  REQUIRE( uitsl::compare_files( "assets/jabberwocky.tar", temp_path ) );

}

TEST_CASE("inflate nested.tar.gz") {

  const auto temp_path{ uitsl::inflate( "assets/nested.tar.gz" ) };

  REQUIRE( uitsl::compare_files( "assets/nested.tar", temp_path ) );

}

TEST_CASE("inflate rick.tar.gz") {

  const auto temp_path{ uitsl::inflate( "assets/rick.tar.gz" ) };

  REQUIRE( uitsl::compare_files( "assets/rick.tar", temp_path ) );

}

TEST_CASE("inflate twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son.gz") {

  const auto temp_path{ uitsl::inflate( "assets/twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son.gz" ) };

  REQUIRE( uitsl::compare_files( "assets/twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son", temp_path ) );

}

TEST_CASE("inflate zen.tar.gz") {

  const auto temp_path{ uitsl::inflate( "assets/zen.tar.gz" ) };

  REQUIRE( uitsl::compare_files( "assets/zen.tar", temp_path ) );

}
