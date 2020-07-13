//  This file is part of Pipe Profile
//  Copyright (C) Matthew Andres Moreno, 2020.
//  Released under MIT license; see LICENSE

#include <iostream>
#include <chrono>

#include "../grid_utils.h"
#include "../config_utils.h"

#include "../Tile.h"

int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  std::cout << ">>> begin <<<" << std::endl << std::endl;

  const config_t cfg{make_config()};

  print_config(cfg);

  grid_t grid(
    make_grid(cfg)
  );

  const auto start = std::chrono::high_resolution_clock::now();

  run_grid(grid, cfg);

  const auto stop = std::chrono::high_resolution_clock::now();

  const auto duration = std::chrono::duration_cast<
    std::chrono::nanoseconds
  >(stop - start);

  if (cfg.at("audit")) audit_grid(grid, cfg, duration.count());

  std::cout << ">>> end <<<" << std::endl;

  MPI_Finalize();

}
