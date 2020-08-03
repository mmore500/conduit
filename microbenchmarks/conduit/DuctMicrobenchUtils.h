#pragma once

#include <thread>
#include <tuple>
#include <iostream>

#include <benchmark/benchmark.h>

#include "mpi.h"

#include "meta/TypePack.h"
#include "tools/Random.h"

#include "conduit/config.h"
#include "utility/CircularIndex.h"
#include "parallel/ThreadTeam.h"
#include "mesh/Mesh.h"
#include "utility/TimeGuard.h"
#include "utility/ForEach.h"
#include "utility/Curry.h"

#include "distributed/mpi_utils.h"
#include "conduit/pipe_utils.h"
#include "utility/numeric_cast.h"
#include "mesh/mesh_utils.h"
#include "parallel/thread_utils.h"
#include "utility/benchmark_utils.h"
#include "utility/stats_utils.h"

#include "DuctMicrobenchRunner.h"

#define MESSAGE_T int

template<typename MeshFactory, size_t N>
struct RegisterBenchmarks {

  benchmark::internal::Benchmark * make_benchmark() {

    const std::string name{ MeshFactory::GetName() };

    auto res = benchmark::RegisterBenchmark(
      name.c_str(),
      [](benchmark::State& state){
        static DuctMicrobenchRunner<
          N,
          MeshFactory,
          MESSAGE_T
        > runner{};
        runner.Run(state);
      }
    );

    res->Threads(N);
    uit::report_confidence(res);

    return res;

  }

  RegisterBenchmarks() {

    make_benchmark();
    make_benchmark()->UseRealTime();

  }
};

template<typename MeshFactory, typename Ntype>
struct RegisterBenchmarksTypeTemplate : public RegisterBenchmarks<
  MeshFactory,
  Ntype{}()
> {};

// instantiated for each sample thread counts
template<size_t N>
struct ThreadCountPayload {

  ThreadCountPayload() {

    using factories_t = emp::TypePack<
      uit::RingMeshFactory<MESSAGE_T>,
      uit::DyadicMeshFactory<MESSAGE_T>,
      uit::ProducerConsumerMeshFactory<MESSAGE_T>
    >;

    using benchmarks_t = typename factories_t::wrap<
      uit::Curry<
        RegisterBenchmarksTypeTemplate,
        uit::ValType<N>
      >::template curried
    >;

    using instantiator_t = typename benchmarks_t::template apply<std::tuple>;

    // benchmarks registered in constructors for each type in tuple
    instantiator_t{};

  }
};
