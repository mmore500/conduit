#include <ratio>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/utility/PrefilledMirroredRingBuffer.hpp"

TEST_CASE("Test PrefilledMirroredRingBuffer") {

  constexpr size_t buff_size{ 10 };
  uit::PrefilledMirroredRingBuffer<size_t, buff_size> buff;

  REQUIRE( buff.GetSize() == 10 );

  do {
    if ( buff.GetSize() ) REQUIRE(buff.GetTail() == 0);
  } while( buff.Pop() );


}
