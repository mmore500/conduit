#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <string>
#include <thread>

#include <omp.h>

#include "barrier.h"
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
  const size_t use_omp = cfg.at("use_omp");
  const size_t synchronous = cfg.at("synchronous");

  const auto task_step = [=](chunk_t chunk){
    update_chunk(chunk, verbose, resistance);
  };

  latch latch{numeric_cast<std::ptrdiff_t>(num_threads)};
  barrier barrier{numeric_cast<std::ptrdiff_t>(num_threads)};

  const auto task_sequence = [&](chunk_t chunk){

    emp_assert(!use_omp);

    // synchronize after thread creation
    if (!synchronous) latch.arrive_and_wait();

    for (size_t update = 0; update < num_updates; ++update) {
      task_step(chunk);

      // synchronize after each step
      if (synchronous) barrier.arrive_and_wait();

    }

  };

  const auto omp_sync = [task_step, num_updates, &chunks](){
    /*
    const size_t num_chunks = chunks.size();

    for (size_t update = 0; update < num_updates; ++update) {
        #pragma omp parallel for
        for (size_t i = 0; i < num_chunks; ++i) task_step(chunks[i]);
      }
    }
    */
  };

  const auto omp_async = [task_step, &chunks](){
    /*
    const size_t num_chunks = chunks.size();

    #pragma omp parallel
    {
      // attempt to ensure synchonous thread initialization
      #pragma omp barrier

      #pragma omp for
      for (size_t i = 0; i < num_chunks; ++i) task_step(chunks[i]);
    }
    */
  };

  const auto std_run = [&](){
    std::vector<std::thread> workers;

    for (auto chunk : chunks) {
      workers.emplace_back(
        [chunk, task_sequence](){ task_sequence(chunk); }
      );
    }

    std::for_each(
      std::begin(workers),
      std::end(workers),
      [](auto & worker){ worker.join(); }
    );
  };

  if (use_omp) {
    if (synchronous) omp_sync();
    else omp_async();
  } else std_run();

}
