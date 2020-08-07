#pragma once

#include <assert.h>
#include <thread>
#include <benchmark/benchmark.h>

#include "mpi.h"

#include "conduit/config.h"
#include "utility/CircularIndex.h"
#include "parallel/ThreadTeam.h"
#include "mesh/Mesh.h"
#include "utility/TimeGuard.h"
#include "utility/ForEach.h"

#include "distributed/mpi_utils.h"
#include "conduit/pipe_utils.h"
#include "utility/numeric_cast.h"
#include "mesh/mesh_utils.h"
#include "parallel/thread_utils.h"
#include "utility/benchmark_utils.h"

template<
  size_t num_threads,
  typename MeshFactory,
  typename MESSAGE_T
>
struct DuctMicrobenchRunner {

  uit::Mesh<MESSAGE_T> mesh{
   MeshFactory{}(num_threads), // TODO *2
   uit::assign_segregated<uit::thread_id_t>()
  };

  void Run(benchmark::State& state) {
    uit::io_bundle_t<MESSAGE_T> & bundle = mesh.GetNode(state.thread_index);

    const bool is_producer = bundle.outputs.size();
    const bool is_consumer = bundle.inputs.size();

    auto * const input = is_consumer
      ? &bundle.inputs[0].GetInput()
      : nullptr;
    auto * const output = is_producer
      ? &bundle.outputs[0].GetOutput()
      : nullptr;


    MESSAGE_T last{};
    size_t num_messages{};
    int latency{};

    int epoch{};
    for (auto _ : state) {
      if (is_producer) output->MaybePut(epoch+1);
      if (is_consumer) {
        const MESSAGE_T cur = input->GetCurrent();
        num_messages += (cur != last);
        latency += epoch - cur;
        last = cur;
      }
      ++epoch;
    }

    state.counters.insert({
      {
        "Latency",
        benchmark::Counter(
          latency * num_messages / epoch,
          benchmark::Counter::kAvgThreadsRate
            | benchmark::Counter::kInvert
        )
      },
      {
        "Lossiness",
        benchmark::Counter(
          1.0 - ((double)num_messages)/epoch,
          benchmark::Counter::kAvgThreads
        )
      }
    });


  }

};
