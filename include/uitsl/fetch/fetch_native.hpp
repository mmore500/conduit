#pragma once
#ifndef UITSL_FETCH_FETCH_NATIVE_HPP_INCLUDE
#define UITSL_FETCH_FETCH_NATIVE_HPP_INCLUDE

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

#include <curl/curl.h>

#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"

#include "../debug/err_verify.hpp"
#include "../nonce/ScopeGuard.hpp"
#include "../polyfill/filesystem.hpp"

#include "make_temp_path.hpp"

namespace uitsl {

/*
 * Fetch data from a url to a temporary file, return temporary file path.
 * Adapted from https://stackoverflow.com/a/21573625.
 */
std::filesystem::path fetch_native( const std::string& url ) {

  const uitsl::ScopeGuard curl_global_guard(
    [](){ curl_global_init(CURL_GLOBAL_ALL); },
    [](){ curl_global_cleanup(); }
  );

  const std::filesystem::path outpath{ uitsl::make_temp_path() };

  FILE *outfile;
  const uitsl::ScopeGuard outfile_guard(
    [&outfile, outpath](){ outfile = fopen(outpath.c_str(), "wb"); },
    [&outfile](){ fclose(outfile); }
  );

  CURL* curl_handle;
  const uitsl::ScopeGuard curl_handle_guard(
    [&curl_handle](){ curl_handle = curl_easy_init(); },
    [&curl_handle](){ curl_easy_cleanup( curl_handle ); }
  );

  emp_always_assert( curl_handle );

  uitsl::err_verify(
    curl_easy_setopt( curl_handle, CURLOPT_URL, url.c_str())
  );
  uitsl::err_verify(
    curl_easy_setopt( curl_handle, CURLOPT_WRITEFUNCTION, nullptr)
  );
  uitsl::err_verify(
    curl_easy_setopt( curl_handle, CURLOPT_WRITEDATA, outfile)
  );

  uitsl::err_verify( curl_easy_perform(curl_handle) );

  long http_code{};
  curl_easy_getinfo( curl_handle, CURLINFO_RESPONSE_CODE, &http_code );
  emp_always_assert( http_code == 200, url, http_code );

  return outpath;

}

} // namespace uitsl

#endif // #ifndef UITSL_FETCH_FETCH_NATIVE_HPP_INCLUDE
