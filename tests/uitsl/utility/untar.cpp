#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/compare_files.hpp"
#include "uitsl/utility/untar.hpp"

TEST_CASE("untar file") {

  REQUIRE( uitsl::untar( "assets/rick.tar" ) );

  REQUIRE( uitsl::compare_files( "assets/rick.txt", "rick.txt" ) );

}

TEST_CASE("untar nested directory") {

  REQUIRE( uitsl::untar( "assets/nested.tar" ) );

  REQUIRE( uitsl::compare_files(
    "assets/rick.txt", "never/gonna/let/you/down/rick.txt"
  ) );

}

TEST_CASE("untar empty directory") {

  REQUIRE( uitsl::untar( "assets/empty.tar" ) );

  REQUIRE( std::filesystem::exists( "empty/also_empty" ) );

}

// these fail when running the container with Docker but not with Singularity
// with error message
// ERROR: setting permissions for "./." failed with error code generic:1
// ERROR: untar failure assets/____.tar
// TODO fix
// TEST_CASE("untar long filename") {

//   REQUIRE( uitsl::untar( "assets/jabberwocky.tar" ) );

//   REQUIRE( uitsl::compare_files(
//     "assets/twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son-the-jaws-that-bite-the-claws-that-catch-beware-the-jubjub-bird-and-shun-the-frumious-bandersnatch",
//     "twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son-the-jaws-that-bite-the-claws-that-catch-beware-the-jubjub-bird-and-shun-the-frumious-bandersnatch"
//   ) );

// }

// TEST_CASE("untar long filename and long directory name") {

//   REQUIRE( uitsl::untar( "assets/zen.tar" ) );

//   REQUIRE( uitsl::compare_files(
//     "assets/beautiful-is-better-than-ugly-explicit-is-better-than-implicit-simple-is-better-than-complex-complex-is-better-than-complicated-flat-is-better-than-nested-sparse-is-better-than-dense-readability-counts-special-cases-arent-special-enough-to-break-the-rules/although-practicality-beats-purity-errors-should-never-pass-silently-unless-explicitly-silenced-in-the-face-of-ambiguity-refuse-the-temptation-to-guess-there-should-be-one-and-preferably-only-one-obvious-way-to-do-it",
//     "beautiful-is-better-than-ugly-explicit-is-better-than-implicit-simple-is-better-than-complex-complex-is-better-than-complicated-flat-is-better-than-nested-sparse-is-better-than-dense-readability-counts-special-cases-arent-special-enough-to-break-the-rules/although-practicality-beats-purity-errors-should-never-pass-silently-unless-explicitly-silenced-in-the-face-of-ambiguity-refuse-the-temptation-to-guess-there-should-be-one-and-preferably-only-one-obvious-way-to-do-it"
//   ) );

// }
