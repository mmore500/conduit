#pragma once



#include <unordered_map>

namespace uitsl {

/// Assigns a unique ID (UID) to an ordered tuple of elements.
template <typename T>
class UIDMap {
  using node_id_t = size_t;
  using node_tuple = std::tuple<bool, T, T>;

  size_t counter{};

  std::unordered_multimap<
    node_tuple,
    size_t,
    emp::TupleHash<bool, T, T>
  > map;

public:
  size_t operator[](const node_tuple& a) {
    const auto& [is_output, from_node, to_node] = a;
    const node_tuple complement{
      !is_output,
      from_node,
      to_node
    };

    if (map.count(complement)) {
      const auto it = map.find(complement);
      const auto [key, val] = *it;
      map.erase(it);
      return val;
    } else {
      return map.insert({a, counter++})->second;
    }
  }
};

} // namespace uitsl

