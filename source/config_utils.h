#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

using config_t = std::unordered_map<std::string, size_t>;


config_t make_config() {

  return config_t{
    {
      "num_chunks",
      std::stoi(
        std::getenv("OMP_NUM_THREADS") ?: "1"
      )
    },
    {
      "num_updates",
      std::stoi(
        std::getenv("PP_NUM_UPDATES") ?: "100"
      )
    },
    {
      "grid_size",
      std::stoi(
        std::getenv("PP_GRID_SIZE") ?: "10"
      )
    },
    {
      "synchronous",
      std::stoi(
        std::getenv("PP_SYNCHRONOUS") ?: "1"
      )
    },
    {
      "resistance",
      std::stoi(
        std::getenv("PP_RESISTANCE") ?: "0"
      )
    },
    {
      "verbose",
      std::stoi(
        std::getenv("PP_VERBOSE") ?: "0"
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
