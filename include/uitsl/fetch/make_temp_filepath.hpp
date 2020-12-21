#pragma once
#ifndef UITSL_FETCH_MAKE_TEMP_FILEPATH_HPP_INCLUDE
#define UITSL_FETCH_MAKE_TEMP_FILEPATH_HPP_INCLUDE

#include <cstdlib>
#include <string>

#include "../debug/err_audit.hpp"
#include "../polyfill/filesystem.hpp"

namespace uitsl {

inline std::filesystem::path make_temp_filepath() {

  // adapted from https://en.wikipedia.org/wiki/Mkstemp
  char dest_path[] = "/tmp/uitsl_temp_fileXXXXXX";
  uitsl::err_audit(! mkstemp(dest_path) );
  return dest_path;

}

} // namespace uitsl

#endif // #ifndef UITSL_FETCH_MAKE_TEMP_FILEPATH_HPP_INCLUDE
