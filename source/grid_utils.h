#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <string>
#include <thread>

#ifdef PP_USE_OMP
#include <omp.h>
#endif

#include "data/DataFile.h"
#include "tools/keyname_utils.h"

#include "barrier.h"
#include "latch.h"
#include "numeric_cast.h"

#include "pipe_utils.h"
#include "config_utils.h"
#include "chunk_utils.h"
#include "mpi_utils.h"

#include "CountdownIterator.h"
#include "CountdownTimer.h"
#include "TimeoutBarrier.h"
#include "Gatherer.h"
#include "Tile.h"
#include "ThreadTeam.h"
#include "State.h"

using grid_t = std::vector<Tile>;
using handle_t = grid_t::iterator;
using chunk_t = std::vector<handle_t>;

grid_t make_grid(const config_t & cfg) {

  std::vector<Inlet<State>> inlets;
  std::vector<Outlet<State>> outlets;

  const size_t grid_size = cfg.at("grid_size");
  for (size_t i = 0; i < grid_size; ++i) {
    auto res = make_pipe<State>();
    auto & [in, out] = res;
    inlets.push_back(in);
    outlets.push_back(out);
  }

  // before rotate
  // outlets: 0 1 2 3  (inputs)
  //          | | | |
  // inlets:  0 1 2 3  (outputs)

  std::rotate(
    std::rbegin(outlets),
    std::rbegin(outlets) + 1,
    std::rend(outlets)
  );

  // after rotate
  // outlets:   0 1 2 3  (inputs)
  //           \ \ \ \
  // inlets:    3 0 1 2  (outputs)

  grid_t grid;

  for (size_t i = 0; i < grid_size; ++i) {
    grid.push_back(
      Tile(
        outlets[i],
        inlets[i]
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

  const std::array<State, 2> states{'_', 'O'};

  for (size_t i = 0; i < grid.size(); ++i) {
    grid[i].SetState(states[i % states.size()]);
  }

}

double run_grid(grid_t & grid, const config_t & cfg) {

  std::vector<chunk_t> chunks(
    make_chunks(
      grid,
      cfg.at("num_chunks")
    )
  );

  if (is_multiprocess()) {

    const size_t prev_proc = circular_index(get_rank(), get_nprocs(), -1);
    grid.front().SplitInputDuct<ProcessOutletDuct<State>>(
      prev_proc,
      prev_proc // tag
    );

    const size_t next_proc = circular_index(get_rank(), get_nprocs(), 1);
    grid.back().SplitOutputDuct<ProcessInletDuct<State>>(
      next_proc,
      get_rank() // tag
    );


  }

  initialize_grid(grid);

  const size_t num_updates = cfg.at("num_updates");
  const size_t num_seconds = cfg.at("num_seconds");
  const size_t verbose = cfg.at("verbose");
  const size_t resistance = cfg.at("resistance");
  const size_t num_threads = cfg.at("num_threads");
  const size_t use_omp = cfg.at("use_omp");
  const size_t synchronous = cfg.at("synchronous");
  const size_t shuffle_tile_evaluation = cfg.at("shuffle_tile_evaluation");
  const size_t checkout_memory = cfg.at("checkout_memory");

  const auto task_step = [=](chunk_t chunk){
    update_chunk(chunk, verbose, shuffle_tile_evaluation, resistance);
  };

  latch latch{numeric_cast<std::ptrdiff_t>(num_threads)};
  barrier barrier{numeric_cast<std::ptrdiff_t>(num_threads)};

  Gatherer<int> gatherer(MPI_INT);

  const auto task_sequence = [&](chunk_t source){

    emp_assert(!use_omp);

    auto chunk = checkout_memory
      ? checkout_chunk(source)
      : source;

    CountdownTimer timer{
      std::chrono::seconds{num_seconds},
      10000
    };
    CountdownIterator counter{num_updates};

    // synchronize once after thread creation and MPI spinup
    if (!synchronous) {
      latch.arrive_and_wait();
      MPI_Barrier(MPI_COMM_WORLD);
    }

    while (!counter.IsComplete() && !timer.IsComplete()) {
      task_step(chunk);

      // synchronize after each step
      if (synchronous) TimeoutBarrier{
        timer,
        barrier
      };

      counter.Step();

    }

    if (synchronous) barrier.arrive_and_drop();

    if (checkout_memory) checkin_chunk(source, chunk);

    gatherer.Put(numeric_cast<int>(
      counter.GetElapsed() / (
        num_seconds
        ?: std::chrono::duration_cast<std::chrono::duration<double>>(
          timer.GetElapsed()
        ).count() ?: 1
      )
    ));

  };

  const auto omp_sync = [task_step, num_updates, &chunks](){
    #ifdef PP_USE_OMP
    const size_t num_chunks = chunks.size();

    for (size_t update = 0; update < num_updates; ++update) {
        #pragma omp parallel for
        for (size_t i = 0; i < num_chunks; ++i) task_step(chunks[i]);
      }
    }
    #endif
  };

  const auto omp_async = [task_step, &chunks](){
    #ifdef PP_USE_OMP
    const size_t num_chunks = chunks.size();

    #pragma omp parallel
    {
      // attempt to ensure synchonous thread initialization
      #pragma omp barrier

      #pragma omp for
      for (size_t i = 0; i < num_chunks; ++i) task_step(chunks[i]);
    }
    #endif
  };

  const auto std_run = [&](){
    ThreadTeam team;

    for (auto chunk : chunks) {
      team.Add([chunk, task_sequence](){ task_sequence(chunk); });
    }

    team.Join();

  };

  if (use_omp) {
    if (synchronous) omp_sync();
    else omp_async();
  } else std_run();

  const auto productivities = gatherer.Gather();

  return productivities
    ? std::accumulate(
      std::begin(*productivities),
      std::end(*productivities),
      0.0
    ) / productivities->size()
    : -1.0;

}

void audit_grid(
  const grid_t & grid,
  const config_t& cfg,
  const size_t nanoseconds
) {

  std::unordered_map<std::string, std::string> descriptors{
    {"title", "audit"},
    {"nanoseconds", emp::to_string(nanoseconds)},
    {"ext", ".csv"}
  };

  std::transform(
    std::begin(cfg),
    std::end(cfg),
    std::inserter(descriptors, std::begin(descriptors)),
    [](const auto & pair){
      const auto & [key, value] = pair;
      return std::pair<std::string, std::string>{key, emp::to_string(value)};
    }
  );

  emp::DataFile datafile(
    emp::keyname::pack(descriptors)
  );

  size_t tile;
  datafile.AddVar<size_t>(tile, "Tile");

  size_t successful_write_count;
  datafile.AddVar<size_t>(successful_write_count, "Successful Write Count");

  size_t blocked_write_count;
  datafile.AddVar<size_t>(blocked_write_count, "Blocked Write Count");

  size_t dropped_write_count;
  datafile.AddVar<size_t>(dropped_write_count, "Dropped Write Count");

  size_t read_count;
  datafile.AddVar<size_t>(read_count, "Read Count");

  size_t read_revision_count;
  datafile.AddVar<size_t>(read_revision_count, "Read Revision Count");

  size_t net_flux;
  datafile.AddVar<size_t>(net_flux, "Net Flux");

  datafile.PrintHeaderKeys();

  for (tile = 0; tile < grid.size(); ++tile) {
    const auto & which = grid[tile];

    successful_write_count = which.GetSuccessfulWriteCount();
    blocked_write_count = which.GetBlockedWriteCount();
    dropped_write_count = which.GetDroppedWriteCount();
    read_count = which.GetReadCount();
    read_revision_count = which.GetReadRevisionCount();
    net_flux = which.GetNetFlux();

    datafile.Update();
  }



}
