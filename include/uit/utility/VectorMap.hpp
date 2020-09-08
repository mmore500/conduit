#pragma once

#include <optional>
#include <utility>

#include "../../../third-party/Empirical/source/base/vector.h"

namespace uit {

namespace internal {

template<typename Key, typename T>
class VectorMapIterator
: public emp::vector<std::optional<std::pair<Key, T>>>::iterator {

  using value_type = std::pair<Key, T>;
  using parent_t = typename emp::vector<std::optional< value_type >>::iterator;

public:

  // inherit parent's constructors
  using parent_t::parent_t;

  const value_type* operator->() const { return *parent_t::operator->(); }

  value_type* operator->() { return *parent_t::operator->(); }

  const value_type& operator*() const& { return *parent_t::operator*(); }

  const value_type& operator*() & { return *parent_t::operator*(); }

  constexpr value_type&& operator*() && {
    return std::move( *parent_t::operator*() );
  }

  constexpr const value_type&& operator*() const&& {
    return std::move( *parent_t::operator*() );
  }

};

} // namespace internal

template<typename Key, typename T>
class VectorMap {

public:

  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<Key, T>;
  using iterator = internal::VectorMapIterator<Key, T>;

private:

  emp::vector< std::optional<
    value_type
  > > data;

  size_t num_items{};

public:

  T& operator[](const Key i) {
    // if needed, make space
    if (i >= data.size()) data.resize(i + 1);

    // if needed, set up new value
    if ( !data[i].has_value() ) {
      data[i].emplace(
        std::piecewise_construct,
        std::forward_as_tuple(i),
        std::forward_as_tuple()
      );
      ++num_items;
    }

    return at(i);
  }

  T& at(const Key i) { return data[i]->second; }

  const T& at(const Key i) const { return data[i]->second; }

  decltype(auto) begin() {
    emp_assert( data.size() == size(), "data is not contiguous" );
    return iterator{ std::begin( data ) };
  }

  decltype(auto) end() {
    emp_assert( data.size() == size(), "data is not contiguous" );
    return iterator{ std::end( data ) };
  }

  decltype(auto) cbegin() const {
    emp_assert( data.size() == size(), "data is not contiguous" );
    return iterator{ std::cbegin( data ) };
  }

  decltype(auto) cend() const {
    emp_assert( data.size() == size(), "data is not contiguous" );
    return iterator{ std::cend( data ) };
  }

  size_t size() const { return num_items; }

  size_t empty() const { return !size(); }

  void erase(const Key i) { data[i].reset(); --num_items; }

  void clear() { data.clear(); num_items = 0; }

  size_t count(const Key i) const {
    return i < data.size() && data[i].has_value();
  }

  bool contains(const Key i) const { return count(i); }

};

} // namespace uit
