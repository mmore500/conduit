#pragma once
#ifndef UITSL_UTILITY_KEYNAME_DIRECTORY_SCRAPE_HPP_INCLUDE
#define UITSL_UTILITY_KEYNAME_DIRECTORY_SCRAPE_HPP_INCLUDE

#include <string>
#include <type_traits>
#include <vector>

#include "../../uit_emp/tools/keyname_utils.hpp"

#include "../polyfill/filesystem.hpp"
#include "../polyfill/identity.hpp"

namespace uitsl {

std::vector< std::filesystem::path > keyname_directory_scrape(
  const std::string& key, const std::filesystem::path& target="."
) {

  std::vector< std::filesystem::path > res;
  std::copy_if(
    std::filesystem::directory_iterator( target ),
    std::filesystem::directory_iterator{},
    std::back_inserter( res ),
    [&]( const auto& entry ){
      return uit_emp::keyname::unpack( entry.path().string() ).count( key );
    }
  );

  return res;

}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_KEYNAME_DIRECTORY_SCRAPE_HPP_INCLUDE
