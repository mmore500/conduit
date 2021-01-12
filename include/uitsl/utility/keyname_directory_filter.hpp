#pragma once
#ifndef UITSL_UTILITY_KEYNAME_DIRECTORY_FILTER_HPP_INCLUDE
#define UITSL_UTILITY_KEYNAME_DIRECTORY_FILTER_HPP_INCLUDE

#include <algorithm>
#include <string>
#include <type_traits>
#include <utility>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "../polyfill/filesystem.hpp"
#include "../polyfill/identity.hpp"

namespace uitsl {

emp::vector< std::filesystem::path > keyname_directory_filter(
  const emp::vector<std::pair<std::string, std::string>>& keyvals,
  const std::filesystem::path& target="."
) {

  emp::vector< std::filesystem::path > res;
  std::copy_if(
    std::filesystem::directory_iterator( target ),
    std::filesystem::directory_iterator{},
    std::back_inserter( res ),
    [&]( const auto& entry ){
      const auto keyname_attrs = emp::keyname::unpack( entry.path().string() );
      return std::all_of(
        std::begin(keyvals),
        std::end(keyvals),
        [&]( const auto& keyval ) {
          const auto& [key, val] = keyval;
          return keyname_attrs.count( key ) && keyname_attrs.at( key ) == val;
        }
      );
    }
  );

  return res;

}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_KEYNAME_DIRECTORY_FILTER_HPP_INCLUDE
