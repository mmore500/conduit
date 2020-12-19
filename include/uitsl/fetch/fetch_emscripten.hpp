#pragma once
#ifndef UITSL_FETCH_FETCH_EMSCRIPTEN_HPP_INCLUDE
#define UITSL_FETCH_FETCH_EMSCRIPTEN_HPP_INCLUDE

#include "../polyfill/filesystem.hpp"

#include "make_temp_path.hpp"

namespace uitsl {

/*
 * Synchronously fetch data from a url to a temporary file,
 * return temporary file path. Requires `xmlhttprequest` package when running
 * with Node.js.
 */
std::filesystem::path fetch_emscripten( const std::string& url ) {

  const std::filesystem::path outpath{ uitsl::make_temp_path() };

  EM_ASM({

    var url = UTF8ToString($0);
    var outfile = UTF8ToString($1);

    if (typeof XMLHttpRequest == "undefined") { // node polyfill
      globalThis.XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;
    }

    var xhr = new XMLHttpRequest();
    xhr.open("GET", url, false);  // synchronous request
    xhr.send();

    console.assert( xhr.status === 200, url, xhr.status );

    FS.writeFile( outfile, xhr.responseText );

  }, url.c_str(), outpath.c_str() );

  return outpath;

}

} // namespace uitsl

#endif // #ifndef UITSL_FETCH_FETCH_EMSCRIPTEN_HPP_INCLUDE
