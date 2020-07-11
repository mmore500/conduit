//  This file is part of Pipe Profile
//  Copyright (C) Matthew Andres Moreno, 2020.
//  Released under MIT license; see LICENSE

#include <iostream>

#include "../grid_utils.h"
#include "../config_utils.h"

#include "../Tile.h"

int main() {

  std::cout << ">>> begin <<<" << std::endl << std::endl;

  const config_t cfg{make_config()};

  print_config(cfg);

  grid_t grid(
    make_grid(cfg)
  );

  initialize_grid(grid);

  run_grid(grid, cfg);

  if (cfg.at("audit")) audit_grid(grid, cfg);

  std::cout << ">>> end <<<" << std::endl;

}
