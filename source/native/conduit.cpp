//  This file is part of Pipe Profile
//  Copyright (C) Matthew Andres Moreno, 2020.
//  Released under MIT license; see LICENSE

#include <iomanip>
#include <iostream>
#include <chrono>

#include "distributed/mpi_utils.hpp"
#include "utility/TimeGuard.hpp"

#include "../config_utils.hpp"
#include "../grid_utils.hpp"
#include "../Tile.hpp"

int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  const config_t cfg{make_config()};

  if (!cfg.at("taciturn")) std::cout << ">>> begin <<<" << std::endl << std::endl;

  if (!cfg.at("taciturn")) print_config(cfg);

  grid_t grid(
    make_grid(cfg)
  );

  std::chrono::seconds duration; {
    const uit::TimeGuard guard(duration);

    const double mean_productivity = run_grid(grid, cfg);

    if (uit::is_root()) {
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
