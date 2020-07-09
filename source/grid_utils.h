#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <string>

#include <omp.h>

#include "pipe_utils.h"
#include "config_utils.h"
#include "chunk_utils.h"

#include "Tile.h"

using grid_t = std::vector<Tile>;
using handle_t = grid_t::iterator;
using chunk_t = std::vector<handle_t>;

grid_t make_grid(const config_t & cfg) {

  std::vector<Inlet<char>> inlets;
  std::vector<Outlet<char>> outlets;

  const size_t grid_size = cfg.at("grid_size");
  for (size_t i = 0; i < grid_size; ++i) {
    auto res = make_pipe<char>();
    auto & [in, out] = res;
    inlets.push_back(in);
    outlets.push_back(out);
  }

  std::rotate(
    std::rbegin(outlets),
    std::rbegin(outlets) + 1,
    std::rend(outlets)
  );

  grid_t grid;

  for (size_t i = 0; i < grid_size; ++i) {
    grid.push_back(
      Tile(
        inlets[i],
        outlets[i]
      )
    );
  }

  for (size_t i = 0; i < grid_size; ++i) {
    grid[i].next = &grid[(i+1)%grid_size];
    grid[i].prev = &grid[(i+grid_size-1)%grid_size];
    grid[i].id = i;
  }


  return grid;

}

void initialize_grid(grid_t & grid) {

  const std::array<char, 2> states{'_', 'O'};

  for (size_t i = 0; i < grid.size(); ++i) {
    grid[i].SetState(states[i % states.size()]);
  }

}

void run_grid(grid_t & grid, const config_t & cfg) {

  std::vector<chunk_t> chunks(
    make_chunks(
      grid,
      cfg.at("num_chunks")
    )
  );

  const size_t num_updates = cfg.at("num_updates");
  const size_t verbose = cfg.at("verbose");

  const auto run_synchronous = [num_updates, verbose, &chunks](){
    for (size_t update = 0; update < num_updates; ++update) {
      #pragma omp parallel for
      for (size_t i = 0; i < chunks.size(); ++i) {
        update_chunk(chunks[i], verbose);
      }
    }
  };

  const auto run_asynchronous = [num_updates, verbose, &chunks](){
    #pragma omp parallel for
    for (size_t i = 0; i < chunks.size(); ++i) {
      for (size_t update = 0; update < num_updates; ++update) {
        update_chunk(chunks[i], verbose);
      }
    }
  };

  const size_t synchronous = cfg.at("synchronous");

  if (synchronous) run_synchronous();
  else run_asynchronous();

}
