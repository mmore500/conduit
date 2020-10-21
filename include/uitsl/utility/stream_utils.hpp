#pragma once
#ifndef UITSL_UTILITY_STREAM_UTILS_HPP_INCLUDE
#define UITSL_UTILITY_STREAM_UTILS_HPP_INCLUDE

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

namespace uitsl {

class Line {
  std::string str;
public:
  Line() = default;

  operator std::string() const { return str; }
  friend std::istream& operator>>(std::istream&, Line&);
};

std::istream& operator>>(std::istream& is, Line& line) {
  std::getline(is, line.str);
  return is;
}

template <typename Out>
void read_lines(std::istream & is, Out out) {
  using In = std::istream_iterator<uitsl::Line>;
  std::copy(In(is), In(), out);
}

bool compare_streams(std::istream& s1, std::istream& s2) {
  // seek back to beginning and use std::equal to compare contents
  s1.seekg(0, std::istream::beg);
  s2.seekg(0, std::istream::beg);

  // stream problem
  if (s1.fail() || s2.fail()) {
    std::err << "stream problem in compare_streams" << std::endl;
    return false;
  }

  //size mismatch
  if (s1.tellg() != s2.tellg()) return false;

  s1.seekg(0, std::istream::beg);
  s2.seekg(0, std::istream::beg);

  return std::equal(
    std::istreambuf_iterator<char>(s1.rdbuf()),
    std::istreambuf_iterator<char>(),
    std::istreambuf_iterator<char>(s2.rdbuf())
  );
}

/// Do files at paths p1 and p2 have identical contents?
bool compare_files(const std::string& p1, const std::string& p2) {
  // adapted from https://stackoverflow.com/a/37575457
  std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
  std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

  return compare_streams(f1, f2);
}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_STREAM_UTILS_HPP_INCLUDE
