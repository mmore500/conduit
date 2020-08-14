#pragma once

#include <stddef.h>

namespace uit {

template<size_t N>
class CircularIndex {

  size_t index{};

public:

  CircularIndex() { ; }

  CircularIndex(size_t index_)
  : index(index_%N)
  { ; }

  CircularIndex(int index_)
  : index((index_%N + N)%N)
  { ; }

  operator size_t() const { return index; }

  CircularIndex& operator++() {
    operator+=(1);
    return *this;
  }

  CircularIndex& operator--() {
    operator-=(1);
    return *this;
  }

  CircularIndex operator++(int) {
    CircularIndex res(*this);
    operator++();
    return res;
  }

  CircularIndex operator--(int) {
    CircularIndex res(*this);
    operator--();
    return res;
  }

  CircularIndex& operator+=(const CircularIndex& rhs) {

    index = (index + rhs) % N;
    return *this;
  }

  CircularIndex& operator-=(const CircularIndex& rhs) {
    index = (index + N - rhs) % N;
    return *this;
  }

  // CircularIndex binary ops

  CircularIndex operator+(const CircularIndex& rhs) {
    CircularIndex res(*this);
    res += rhs;
    return res;
  }

  CircularIndex operator-(const CircularIndex& rhs) {
    CircularIndex res(*this);
    res -= rhs;
    return res;
  }

  // int binary ops

  CircularIndex operator+(const int& rhs) {
    return operator+(CircularIndex{rhs});
  }

  CircularIndex operator-(const int& rhs) {
    return operator-(CircularIndex{rhs});
  }

  // size_t binary ops

  CircularIndex operator+(const size_t& rhs) {
    return operator+(CircularIndex{rhs});
  }

  CircularIndex operator-(const size_t& rhs) {
    return operator-(CircularIndex{rhs});
  }

  std::string ToString() const {
    return std::to_string(index);
  }


};

}
