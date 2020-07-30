#pragma once

#include <cmath>
#include <limits>
#include <bitset>

#include "tools/math.h"

#include "utility/numeric_cast.h"

size_t mod(int in_val, const size_t mod_val) {
  emp_assert(mod_val > 0);
  const int signed_mod_val = std::min(
    numeric_cast<size_t>(std::numeric_limits<int>::max()),
    mod_val
  );
  in_val %= signed_mod_val;
  return (in_val < 0) ? (in_val + signed_mod_val) : in_val;
}

size_t circular_index(const size_t pos, const size_t len, const int diff) {
  emp_assert(len > 0);
  return (pos + mod(diff, len)) % len;
}

size_t stoszt(const std::string & source) {
  std::stringstream ss{source};
  size_t res;
  ss >> res;
  return res;
}

template<typename TYPE>
constexpr bool is_power_of_two(TYPE x) {
  return x > 0 && emp::CountOnes(x) == 1;
}

template<typename TYPE>
constexpr int num_bits(TYPE x) {
  return x ? emp::IntLog2(emp::Abs(x)) + 1 : 0;
}

template<typename TYPE>
bool test_bit(const TYPE x, const size_t bit) {
  return std::bitset<sizeof(TYPE)*8>(emp::Abs(x)).test(bit);
}

size_t difference(const size_t a, const size_t b) {
  return std::max(a, b) - std::min(a, b);
}
