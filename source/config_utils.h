#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include <limits>

#define DEFAULT_BUFFER 1024

using config_t = std::unordered_map<std::string, size_t>;

size_t stoszt(const std::string & source) {
  std::stringstream ss{source};
  size_t res;
  ss >> res;
  return res;
}

config_t make_config() {

  return config_t{
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
        std::getenv("PP_NUM_SECONDS") ?: "10"
      )
    },
    {
      "grid_size",
      stoszt(
        std::getenv("PP_GRID_SIZE") ?: "10"
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

}

void print_config(const config_t & cfg) {
  std::cout << ">>> config <<<" << std::endl;
  for (const auto & [name, value] : cfg) {
    std::cout << name << ": " << value << std::endl;
  }
  std::cout << std::endl;
}
