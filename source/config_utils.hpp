#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include <limits>
#include <assert.h>
#include <new>

#include "conduit/ImplSpec.hpp"
#include "../third-party/Empirical/source/tools/string_utils.h"
#include "utility/math_utils.hpp"

#include "State.hpp"

using Spec = uit::ImplSpec<State>;

using config_t = std::unordered_map<std::string, size_t>;

// TODO refactor to Empirical config system
config_t make_config() {

  const config_t res{
    {
      "audit",
      uit::stoszt(
        std::getenv("PP_AUDIT") ?: "0"
      )
    },
    {
      "checkout_memory",
      uit::stoszt(
        std::getenv("PP_CHECKOUT_MEMORY") ?: "0"
      )
    },
    {
      "shuffle_tile_evaluation",
      uit::stoszt(
        std::getenv("PP_SHUFFLE_TILE_EVALUATION") ?: "1"
      )
    },
    {
      "num_chunks",
      uit::stoszt(
        std::getenv("PP_NUM_THREADS") ?: "1"
      )
    },
    {
      "num_threads",
      uit::stoszt(
        std::getenv("PP_NUM_THREADS") ?: "1"
      )
    },
    {
      "num_updates", // undefined = no limit
      uit::stoszt(
        std::getenv("PP_NUM_UPDATES")
        ?: emp::to_string(std::numeric_limits<size_t>::max())
      )
    },
    {
      "num_seconds", // 0 for no limit
      uit::stoszt(
        std::getenv("PP_NUM_SECONDS") ?: "5"
      )
    },
    {
      "grid_size",
      uit::stoszt(
        std::getenv("PP_GRID_SIZE") ?: "1000"
      )
    },
    {
      "synchronous",
      uit::stoszt(
        std::getenv("PP_SYNCHRONOUS") ?: "1"
      )
    },
    {
      "resistance",
      uit::stoszt(
        std::getenv("PP_RESISTANCE") ?: "0"
      )
    },
    {
      "use_omp",
      uit::stoszt(
        std::getenv("PP_USE_OMP") ?: "0"
      )
    },
    {
      "verbose", // printing CA grid
      uit::stoszt(
        std::getenv("PP_VERBOSE") ?: "0"
      )
    },
    {
      "taciturn", // RE: printing other info
      uit::stoszt(
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
