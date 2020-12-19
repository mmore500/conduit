#pragma once
#ifndef UITSL_FETCH_MAKE_TEMP_DIR_HPP_INCLUDE
#define UITSL_FETCH_MAKE_TEMP_DIR_HPP_INCLUDE

#include <cstdlib>
#include <string>

#include "../debug/err_audit.hpp"
#include "../polyfill/filesystem.hpp"

namespace uitsl {

std::filesystem::path make_temp_dirpath() {

  // adapted from https://en.wikipedia.org/wiki/Mkstemp
  char dest_path[] = "/tmp/uitsl_temp_dirXXXXXX";
  uitsl::err_audit(! mkdtemp(dest_path) );
  return dest_path;

}

} // namespace uitsl

#endif // #ifndef UITSL_FETCH_MAKE_TEMP_DIR_HPP_INCLUDE
