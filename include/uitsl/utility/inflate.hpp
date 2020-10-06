#pragma once
#ifndef UITSL_UTILITY_INFLATE_HPP_INCLUDE
#define UITSL_UTILITY_INFLATE_HPP_INCLUDE

#include <cstdio>
#include <cstdlib>
#include <string>

#include "zlib.h"

#include "../debug/err_audit.hpp"
#include "../polyfill/filesystem.hpp"
#include "../nonce/ScopeGuard.hpp"

#define UITSL_INFLATE_CHUNK 16384

namespace uitsl {

/*
 * Decompress from file source to file dest until stream ends or EOF.
 *
 * Adapted from
 * - https://zlib.net/zlib_how.html
 * - https://windrealm.org/tutorials/decompress-gzip-stream.php
 * - https://stackoverflow.com/questions/10195343/copy-a-file-in-a-sane-safe-and-efficient-way
 */
void inflate(gzFile_s* source_handle, FILE* dest_handle) {

 unsigned char buf[UITSL_INFLATE_CHUNK];

 for (
   int size = gzread(source_handle, buf, UITSL_INFLATE_CHUNK);
   size > 0;
   size = gzread(source_handle, buf, UITSL_INFLATE_CHUNK)
 ) std::fwrite(buf, 1, UITSL_INFLATE_CHUNK, dest_handle);

}

std::filesystem::path inflate(
  const std::filesystem::path& source_path,
  const std::filesystem::path& dest_path
) {

  auto source_handle{ gzopen(source_path.c_str(), "rb") };
  const uitsl::ScopeGuard source_guard{
    [](){}, [&source_handle](){ gzclose( source_handle ); }
  };

  auto dest_handle{ std::fopen( dest_path.c_str(), "wb+" ) };
  const uitsl::ScopeGuard dest_guard{
    [](){}, [&dest_handle](){ std::fclose( dest_handle ); }
  };

  inflate( source_handle, dest_handle );

  return dest_path;

}

std::filesystem::path inflate( const std::filesystem::path& source_path ) {
  // adapted from https://en.wikipedia.org/wiki/Mkstemp
  char dest_path[] = "/tmp/uitsl_inflate_dataXXXXXX";
  uitsl::err_audit(! mkstemp(dest_path) );
  return inflate( source_path, dest_path );
}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_INFLATE_HPP_INCLUDE
