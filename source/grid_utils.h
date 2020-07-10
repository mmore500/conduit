#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <string>
#include <thread>

#include <omp.h>

#include "latch.h"
#include "numeric_cast.h"

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
  const size_t resistance = cfg.at("resistance");
  const size_t num_threads = cfg.at("num_threads");

  const auto sync_task = [verbose, resistance](chunk_t chunk){
    update_chunk(chunk, verbose, resistance);
  };


  latch latch{numeric_cast<std::ptrdiff_t>(num_threads)};

  const auto async_task = [
    num_updates,
    num_threads,
    verbose,
    resistance,
    &latch
  ](chunk_t chunk){
    // synchronize after thread creation
    latch.arrive_and_wait();

    for (size_t update = 0; update < num_updates; ++update) {
      update_chunk(chunk, verbose, resistance);
    }
  };

  const auto omp_sync = [sync_task, num_updates, &chunks](){
    /*
    const size_t num_chunks = chunks.size();

    for (size_t update = 0; update < num_updates; ++update) {
      #pragma omp parallel
      {
        // attempt to ensure synchonous thread initialization
        #pragma omp barrier

        #pragma omp for
        for (size_t i = 0; i < num_chunks; ++i) sync_task(chunks[i]);
      }
    }
    */
  };

  const auto omp_async = [async_task, &chunks](){
    /*
    const size_t num_chunks = chunks.size();

    #pragma omp parallel
    {
      // attempt to ensure synchonous thread initialization
      #pragma omp barrier

      #pragma omp for
      for (size_t i = 0; i < num_chunks; ++i) async_task(chunks[i]);
    }
    */
  };

  const auto std_async = [async_task, &chunks](){

    std::vector<std::thread> workers;
    for (auto chunk : chunks) {
      workers.emplace_back(
        [chunk, async_task](){ async_task(chunk); }
      );
    }

    std::for_each(
      std::begin(workers),
      std::end(workers),
      [](auto & worker){ worker.join(); }
    );

  };

  const size_t synchronous = cfg.at("synchronous");
  const size_t use_omp = cfg.at("use_omp");

  if (use_omp) {
    if (synchronous) omp_sync();
    else omp_async();
  } else {
    if (synchronous) assert(false);
    else std_async();
  }

}
