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
struct ThreadDuctMicrobenchRunner {

  Mesh<MESSAGE_T> mesh{
   MeshFactory{}(num_threads),
   assign_segregated<thread_id_t>()
  };

  void Run(benchmark::State& state) {
    io_bundle_t<MESSAGE_T> & bundle = mesh.GetNode(state.thread_index);

    const bool is_producer = bundle.outputs.size();
    const bool is_consumer = bundle.inputs.size();

    auto * const input = is_consumer
      ? &bundle.inputs[0].GetInput()
      : nullptr;
    auto * const output = is_producer
      ? &bundle.outputs[0].GetOutput()
      : nullptr;


    // TODO add counters for num unique received
    for (auto _ : state) {
      if (is_producer) output->MaybePut(state.thread_index);
      if (is_consumer) do_not_optimize(
        input->GetCurrent()
      );
    }

  }

};
