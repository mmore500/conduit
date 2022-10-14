#pragma once
#ifndef UITSL_UTILITY_KEYNAME_DIRECTORY_MIN_HPP_INCLUDE
#define UITSL_UTILITY_KEYNAME_DIRECTORY_MIN_HPP_INCLUDE

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

#include "../polyfill/filesystem.hpp"

#include "keyname_directory_transform.hpp"

namespace uitsl {

template< typename T >
auto keyname_directory_min(
  const std::string& key,
  const std::vector<std::pair<std::string, std::string>>& filters={},
  const std::filesystem::path& target=".",
  const T& parser=std::identity
) {

  const auto transformed = uitsl::keyname_directory_transform(
    key, filters, target, parser
  );

  assert( transformed.size() );

  return *std::min_element(std::begin( transformed ), std::end( transformed ));

}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_KEYNAME_DIRECTORY_MIN_HPP_INCLUDE
