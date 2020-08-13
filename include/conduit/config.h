#pragma once

#include <stddef.h>
#include <new>

namespace uit {

// TODO don't use macros
#define DEFAULT_BUFFER 64

#ifdef __cpp_lib_hardware_interference_size
  static constexpr size_t CACHE_LINE_SIZE =
    std::hardware_destructive_interference_size;
#else
  static constexpr size_t CACHE_LINE_SIZE = 64;
#endif

}
