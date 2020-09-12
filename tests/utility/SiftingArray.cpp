#include <ratio>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/utility/SiftingArray.hpp"

TEST_CASE("Test SiftingArray") {

  constexpr size_t buff_size{ 10 };
  uit::SiftingArray<size_t, buff_size> buff;

  REQUIRE( buff.GetSize() == 0 );

  for (size_t i = 0; i < buff_size; ++i) {
    REQUIRE( buff.GetSize() == i );
    buff.PushBack( i );
    REQUIRE( buff.Front() == 0 );
    REQUIRE( buff.Back() == i );
    REQUIRE( buff.Get( i ) == i );
  }

  REQUIRE( buff.GetSize() == buff_size );

  buff.SiftByIndex([](const size_t i) -> bool { return i % 2; });

  REQUIRE( buff.GetSize() == 5 );
  for (size_t i : std::vector<size_t>{1, 3, 5, 7, 9}) {
    REQUIRE( std::find(
      std::begin(buff),
      std::end(buff),
      i
    ) != std::end(buff) );
  }

}
