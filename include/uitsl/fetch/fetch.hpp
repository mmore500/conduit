#pragma once
#ifndef UITSL_FETCH_FETCH_HPP_INCLUDE
#define UITSL_FETCH_FETCH_HPP_INCLUDE

#include <cstdio>
#include <cstdlib>
#include <string>

#include "../polyfill/filesystem.hpp"

#ifdef __EMSCRIPTEN__
  #include "fetch_emscripten.hpp"
#else
  #include "fetch_native.hpp"
#endif

namespace uitsl {

/*
 * TODO
 */
std::filesystem::path fetch( const std::string& url ) {
  #ifdef __EMSCRIPTEN__
    return uitsl::fetch_emscripten( url );
  #else
    return uitsl::fetch_native( url );
  #endif
}

} // namespace uitsl

#endif // #ifndef UITSL_FETCH_FETCH_HPP_INCLUDE
