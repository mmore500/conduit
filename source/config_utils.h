#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include <limits>
#include <assert.h>
#include <new>

#include "math_utils.h"

#define DEFAULT_BUFFER 1024

#ifdef __cpp_lib_hardware_interference_size
  static constexpr size_t CACHE_LINE_SIZE =
    std::hardware_destructive_interference_size;
#else
  static constexpr size_t CACHE_LINE_SIZE = 64;
#endif

using config_t = std::unordered_map<std::string, size_t>;

config_t make_config() {

  const config_t res{
    {
      "audit",
      stoszt(
        std::getenv("PP_AUDIT") ?: "0"
      )
    },
    {
      "checkout_memory",
      stoszt(
        std::getenv("PP_CHECKOUT_MEMORY") ?: "0"
      )
    },
    {
      "shuffle_tile_evaluation",
      stoszt(
        std::getenv("PP_SHUFFLE_TILE_EVALUATION") ?: "1"
      )
    },
    {
      "num_chunks",
      stoszt(
        std::getenv("PP_NUM_THREADS") ?: "1"
      )
    },
    {
      "num_threads",
      stoszt(
        std::getenv("PP_NUM_THREADS") ?: "1"
      )
    },
    {
      "num_updates", // undefined = no limit
      stoszt(
        std::getenv("PP_NUM_UPDATES")
        ?: emp::to_string(std::numeric_limits<size_t>::max())
      )
    },
    {
      "num_seconds", // 0 for no limit
      stoszt(
        std::getenv("PP_NUM_SECONDS") ?: "5"
      )
    },
    {
      "grid_size",
      stoszt(
        std::getenv("PP_GRID_SIZE") ?: "1000"
      )
    },
    {
      "synchronous",
      stoszt(
        std::getenv("PP_SYNCHRONOUS") ?: "1"
      )
    },
    {
      "resistance",
      stoszt(
        std::getenv("PP_RESISTANCE") ?: "0"
      )
    },
    {
      "use_omp",
      stoszt(
        std::getenv("PP_USE_OMP") ?: "0"
      )
    },
    {
      "verbose", // printing CA grid
      stoszt(
        std::getenv("PP_VERBOSE") ?: "0"
      )
    },
    {
      "taciturn", // RE: printing other info
      stoszt(
        std::getenv("PP_TACITURN") ?: "0"
      )
    }
  };

  assert(res.at("grid_size") % res.at("num_threads") == 0);

  return res;

}

void print_config(const config_t & cfg) {
  std::cout << ">>> config <<<" << std::endl;
  for (const auto & [name, value] : cfg) {
    std::cout << name << ": " << value << std::endl;
  }
  std::cout << std::endl;
}
