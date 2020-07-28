//  This file is part of Pipe Profile
//  Copyright (C) Matthew Andres Moreno, 2020.
//  Released under MIT license; see LICENSE

#include <iomanip>
#include <iostream>
#include <chrono>

#include "../grid_utils.h"
#include "../config_utils.h"
#include "distributed/mpi_utils.h"

#include "../Tile.h"
#include "utility/TimeGuard.h"

int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  const config_t cfg{make_config()};

  if (!cfg.at("taciturn")) std::cout << ">>> begin <<<" << std::endl << std::endl;

  if (!cfg.at("taciturn")) print_config(cfg);

  grid_t grid(
    make_grid(cfg)
  );

  std::chrono::seconds duration; {
    const TimeGuard guard(duration);

    const double mean_productivity = run_grid(grid, cfg);

    if (is_root()) {
      if (!cfg.at("taciturn")) std::cout << "mean_productivity: ";
      std::cout << std::fixed << std::setprecision(0);
      std::cout << mean_productivity << std::endl;
    }

  }

  if (cfg.at("audit")) audit_grid(
    grid,
    cfg,
    std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()
  );

  if (!cfg.at("taciturn")) std::cout << ">>> end <<<" << std::endl;

  MPI_Finalize();

}
