#pragma once

#include <algorithm>
#include <unordered_map>

namespace uitsl {

template<class It>
typename It::value_type get_plurality(It begin, It end) {

  using T = typename It::value_type;

  std::unordered_map<T, size_t> counter;

  for (auto it = begin; it != end; ++it) ++counter[*it];

  return std::max_element(
    std::begin(counter),
    std::end(counter),
    [](const auto& left, const auto& right){
      return left.second < right.second;
    }
  )->first;

}

} // namespace uitsl
