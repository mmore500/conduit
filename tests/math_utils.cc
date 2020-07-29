#include <limits>

#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "utility/math_utils.h"

TEST_CASE("Test mod") {

  REQUIRE( mod(-11, 10) == 9 );
  REQUIRE( mod(-10, 10) == 0 );
  REQUIRE( mod(-9, 10) == 1 );

  REQUIRE( mod(-5, 10) == 5 );
  REQUIRE( mod(-4, 10) == 6 );
  REQUIRE( mod(-3, 10) == 7 );
  REQUIRE( mod(-2, 10) == 8 );
  REQUIRE( mod(-1, 10) == 9 );
  REQUIRE( mod(0, 10) == 0 );
  REQUIRE( mod(1, 10) == 1 );
  REQUIRE( mod(2, 10) == 2 );
  REQUIRE( mod(3, 10) == 3 );
  REQUIRE( mod(4, 10) == 4 );

  REQUIRE( mod(9, 10) == 9 );
  REQUIRE( mod(10, 10) == 0 );
  REQUIRE( mod(11, 10) == 1 );

}

TEST_CASE("Test circular_index") {

  REQUIRE( circular_index(0, 10, 0) == 0 );

  REQUIRE( circular_index(0, 10, 1) == 1 );

  REQUIRE( circular_index(0, 10, -1) == 9 );

  REQUIRE( circular_index(0, 10, 2) == 2 );

  REQUIRE( circular_index(2, 10, 8) == 0 );

  REQUIRE( circular_index(3, 1, 0) == 0 );

  REQUIRE( circular_index(3, 1, 1) == 0 );

  REQUIRE( circular_index(3, 1, -1) == 0 );

  REQUIRE( circular_index(3, 2, 0) == 1 );

  REQUIRE( circular_index(3, 1, 1) == 0 );

  REQUIRE( circular_index(3, 1, -1) == 0 );

}

TEST_CASE("Test stoszt") {

  REQUIRE( stoszt("0") == 0 );

  REQUIRE( stoszt("1") == 1 );

  REQUIRE( stoszt("2") == 2 );

  REQUIRE( stoszt("42") == 42 );

  REQUIRE( stoszt("4294967296") == 4294967296 );

}

TEST_CASE("Test is_power_of_two") {

  // int
  REQUIRE( !is_power_of_two(-65538) );
  REQUIRE( !is_power_of_two(-65537) );
  REQUIRE( !is_power_of_two(-65536) );
  REQUIRE( !is_power_of_two(-65535) );
  REQUIRE( !is_power_of_two(-65534) );

  REQUIRE( !is_power_of_two(-8) );
  REQUIRE( !is_power_of_two(-7) );
  REQUIRE( !is_power_of_two(-6) );
  REQUIRE( !is_power_of_two(-5) );
  REQUIRE( !is_power_of_two(-4) );
  REQUIRE( !is_power_of_two(-3) );
  REQUIRE( !is_power_of_two(-2) );
  REQUIRE( !is_power_of_two(-1) );
  REQUIRE( !is_power_of_two(0) );
  REQUIRE( is_power_of_two(1) );
  REQUIRE( is_power_of_two(2) );
  REQUIRE( !is_power_of_two(3) );
  REQUIRE( is_power_of_two(4) );
  REQUIRE( !is_power_of_two(5) );
  REQUIRE( !is_power_of_two(6) );
  REQUIRE( !is_power_of_two(7) );
  REQUIRE( is_power_of_two(8) );

  REQUIRE( !is_power_of_two(65534) );
  REQUIRE( !is_power_of_two(65535) );
  REQUIRE( is_power_of_two(65536) );
  REQUIRE( !is_power_of_two(65537) );
  REQUIRE( !is_power_of_two(65538) );

  // size_t
  REQUIRE( !is_power_of_two( (size_t) 0) );
  REQUIRE( is_power_of_two( (size_t) 1) );
  REQUIRE( is_power_of_two( (size_t) 2) );
  REQUIRE( !is_power_of_two( (size_t) 3) );
  REQUIRE( is_power_of_two( (size_t) 4) );
  REQUIRE( !is_power_of_two( (size_t) 5) );
  REQUIRE( !is_power_of_two( (size_t) 6) );
  REQUIRE( !is_power_of_two( (size_t) 7) );
  REQUIRE( is_power_of_two( (size_t) 8) );

  REQUIRE( !is_power_of_two( (size_t) 65534) );
  REQUIRE( !is_power_of_two( (size_t) 65535) );
  REQUIRE( is_power_of_two( (size_t) 65536) );
  REQUIRE( !is_power_of_two( (size_t) 65537) );
  REQUIRE( !is_power_of_two( (size_t) 65538) );

  REQUIRE( !is_power_of_two( (size_t) std::numeric_limits<size_t>::max()) );

  // char
  REQUIRE( !is_power_of_two( (char) -8) );
  REQUIRE( !is_power_of_two( (char) -7) );
  REQUIRE( !is_power_of_two( (char) -6) );
  REQUIRE( !is_power_of_two( (char) -5) );
  REQUIRE( !is_power_of_two( (char) -4) );
  REQUIRE( !is_power_of_two( (char) -3) );
  REQUIRE( !is_power_of_two( (char) -2) );
  REQUIRE( !is_power_of_two( (char) -1) );
  REQUIRE( !is_power_of_two( (char) 0) );
  REQUIRE( is_power_of_two( (char) 1) );
  REQUIRE( is_power_of_two( (char) 2) );
  REQUIRE( !is_power_of_two( (char) 3) );
  REQUIRE( is_power_of_two( (char) 4) );
  REQUIRE( !is_power_of_two( (char) 5) );
  REQUIRE( !is_power_of_two( (char) 6) );
  REQUIRE( !is_power_of_two( (char) 7) );
  REQUIRE( is_power_of_two( (char) 8) );

}

TEST_CASE("Test num_bits") {

  // int
  REQUIRE( num_bits(-65538) == 17 );
  REQUIRE( num_bits(-65537) == 17 );
  REQUIRE( num_bits(-65536) == 17 );
  REQUIRE( num_bits(-65535) == 16 );
  REQUIRE( num_bits(-65534) == 16 );

  REQUIRE( num_bits(-8) == 4 );
  REQUIRE( num_bits(-7) == 3 );
  REQUIRE( num_bits(-6) == 3 );
  REQUIRE( num_bits(-5) == 3 );
  REQUIRE( num_bits(-4) == 3 );
  REQUIRE( num_bits(-3) == 2 );
  REQUIRE( num_bits(-2) == 2 );
  REQUIRE( num_bits(-1) == 1 );
  REQUIRE( num_bits(0) == 0 );
  REQUIRE( num_bits(1) == 1 );
  REQUIRE( num_bits(2) == 2 );
  REQUIRE( num_bits(3) == 2 );
  REQUIRE( num_bits(4) == 3 );
  REQUIRE( num_bits(5) == 3 );
  REQUIRE( num_bits(6) == 3 );
  REQUIRE( num_bits(7) == 3 );
  REQUIRE( num_bits(8) == 4 );

  REQUIRE( num_bits(65534) == 16 );
  REQUIRE( num_bits(65535) == 16 );
  REQUIRE( num_bits(65536) == 17 );
  REQUIRE( num_bits(65537) == 17 );
  REQUIRE( num_bits(65538) == 17 );

  // size_t
  REQUIRE( num_bits( (size_t) 0) == 0 );
  REQUIRE( num_bits( (size_t) 1) == 1 );
  REQUIRE( num_bits( (size_t) 2) == 2 );
  REQUIRE( num_bits( (size_t) 3) == 2 );
  REQUIRE( num_bits( (size_t) 4) == 3 );
  REQUIRE( num_bits( (size_t) 5) == 3 );
  REQUIRE( num_bits( (size_t) 6) == 3 );
  REQUIRE( num_bits( (size_t) 7) == 3 );
  REQUIRE( num_bits( (size_t) 8) == 4 );

  REQUIRE( num_bits( (size_t) 65534) == 16 );
  REQUIRE( num_bits( (size_t) 65535) == 16 );
  REQUIRE( num_bits( (size_t) 65536) == 17 );
  REQUIRE( num_bits( (size_t) 65537) == 17 );
  REQUIRE( num_bits( (size_t) 65538) == 17 );

  // char
  REQUIRE( num_bits( (char) -8) == 4 );
  REQUIRE( num_bits( (char) -7) == 3 );
  REQUIRE( num_bits( (char) -6) == 3 );
  REQUIRE( num_bits( (char) -5) == 3 );
  REQUIRE( num_bits( (char) -4) == 3 );
  REQUIRE( num_bits( (char) -3) == 2 );
  REQUIRE( num_bits( (char) -2) == 2 );
  REQUIRE( num_bits( (char) -1) == 1 );
  REQUIRE( num_bits( (char) 0) == 0 );
  REQUIRE( num_bits( (char) 1) == 1 );
  REQUIRE( num_bits( (char) 2) == 2 );
  REQUIRE( num_bits( (char) 3) == 2 );
  REQUIRE( num_bits( (char) 4) == 3 );
  REQUIRE( num_bits( (char) 5) == 3 );
  REQUIRE( num_bits( (char) 6) == 3 );
  REQUIRE( num_bits( (char) 7) == 3 );
  REQUIRE( num_bits( (char) 8) == 4 );

}

TEST_CASE("test_bit") {

  // int
  REQUIRE( test_bit(-8, 8) == 0 );
  REQUIRE( test_bit(-8, 3) == 1 );
  REQUIRE( test_bit(-2, 0) == 0 );
  REQUIRE( test_bit(-2, 1) == 1 );
  REQUIRE( test_bit(-1, 0) == 1 );
  REQUIRE( test_bit(-1, 1) == 0 );
  REQUIRE( test_bit(0, 0) == 0 );
  REQUIRE( test_bit(0, 1) == 0 );
  REQUIRE( test_bit(0, 2) == 0 );
  REQUIRE( test_bit(1, 0) == 1 );
  REQUIRE( test_bit(1, 2) == 0 );

  REQUIRE( test_bit(8, 15) == 0 );
  REQUIRE( test_bit(8, 3) == 1 );

  // size_t
  REQUIRE( test_bit( (size_t) 0, 0) == 0 );
  REQUIRE( test_bit( (size_t) 0, 1) == 0 );
  REQUIRE( test_bit( (size_t) 0, 2) == 0 );
  REQUIRE( test_bit( (size_t) 1, 0) == 1 );
  REQUIRE( test_bit( (size_t) 1, 2) == 0 );
  REQUIRE( test_bit( (size_t) 3, 1) == 1 );
  REQUIRE( test_bit( (size_t) 3, 0) == 1 );
  REQUIRE( test_bit( (size_t) 3, 2) == 0 );
  REQUIRE( test_bit( (size_t) 3, 27) == 0 );
  REQUIRE( test_bit( (size_t) 8, 15) == 0 );
  REQUIRE( test_bit( (size_t) 8, 3) == 1 );

  for (size_t i = 0; i < sizeof(size_t)*8; ++i) {
    REQUIRE( test_bit(std::numeric_limits<size_t>::max(), i) == 1);
    REQUIRE( test_bit( (size_t) 0, i) == 0);
  }

  // char
  REQUIRE( test_bit( (char) -8, 7) == 0 );
  REQUIRE( test_bit( (char) -7, 2) == 1 );
  REQUIRE( test_bit( (char) -7, 3) == 0 );
  REQUIRE( test_bit( (char) -7, 1) == 1 );
  REQUIRE( test_bit( (char) -2, 0) == 0 );
  REQUIRE( test_bit( (char) -2, 1) == 1 );
  REQUIRE( test_bit( (char) -1, 0) == 1 );
  REQUIRE( test_bit( (char) -1, 1) == 0 );
  REQUIRE( test_bit( (char) 0, 0) == 0 );
  REQUIRE( test_bit( (char) 0, 1) == 0 );
  REQUIRE( test_bit( (char) 0, 2) == 0 );
  REQUIRE( test_bit( (char) 1, 0) == 1 );
  REQUIRE( test_bit( (char) 1, 1) == 0 );
  REQUIRE( test_bit( (char) 3, 1) == 1 );
  REQUIRE( test_bit( (char) 3, 2) == 0 );
  REQUIRE( test_bit( (char) 3, 0) == 1 );
  REQUIRE( test_bit( (char) 3, 5) == 0 );
  REQUIRE( test_bit( (char) 8, 5) == 0 );
  REQUIRE( test_bit( (char) 8, 3) == 1 );

}
