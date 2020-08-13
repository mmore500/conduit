#pragma once

#include <list>
#include <thread>
#include <tuple>
#include <iostream>

#include <benchmark/benchmark.h>

#include <mpi.h>

#include "meta/TypePack.h"
#include "tools/Random.h"
#include "tools/string_utils.h"

#include "polyfill/barrier.h"
#include "conduit/config.h"
#include "utility/CircularIndex.h"
#include "parallel/ThreadTeam.h"
#include "mesh/Mesh.h"
#include "utility/TimeGuard.h"
#include "utility/ForEach.h"
#include "utility/Curry.h"

#include "distributed/mpi_utils.h"
#include "conduit/Conduit.h"
#include "conduit/ImplSpec.h"
#include "utility/numeric_cast.h"
#include "topology/RingTopologyFactory.h"
#include "topology/DyadicTopologyFactory.h"
#include "topology/ProConTopologyFactory.h"
#include "parallel/thread_utils.h"
#include "utility/benchmark_utils.h"
#include "utility/stats_utils.h"

#include "DuctBenchmarkRunner.h"

using Spec = uit::ImplSpec<int, DEFAULT_BUFFER, ImplSel>;

template<
  typename MeshFactory,
  typename NumThreadsType
>
struct RegisterBenchmarks {

  RegisterBenchmarks() {

    const std::string name{ emp::to_string(
      MeshFactory::GetName(),
      "/nodes:",
      NumThreadsType{}() * uit::get_nprocs()
    ) };

    auto res = benchmark::RegisterBenchmark(
      name.c_str(),
      [](benchmark::State& state){

        // make a fresh runner each time
        static std::list<DuctMicrobenchRunner<
          NumThreadsType,
          MeshFactory,
          Spec
        >> dispenser;
        if (state.thread_index == 0) dispenser.emplace_back();

        // wait until new runner is ready
        static std::barrier barrier{NumThreadsType{}()};
        barrier.arrive_and_wait();

        dispenser.back().Run(state);

      }
    );

    res->Threads(NumThreadsType{}());
    uit::report_confidence(res);

    // need to guarantee a fixed number of stop/starts for the
    // MPI barriers to sync up correctly
    if (uit::is_multiprocess()) res->Iterations(100 * std::kilo{}.num);
  }

};

// instantiated for each sampled thread count
template<size_t NumThreads>
struct ThreadCountPayload {

  ThreadCountPayload() {

    using factories_t = emp::TypePack<
      uit::RingTopologyFactory,
      uit::DyadicTopologyFactory,
      uit::ProConTopologyFactory
    >;

    using benchmarks_t = typename factories_t::wrap<
      uit::Curry<
        RegisterBenchmarks,
        uit::ValType<NumThreads>
      >::template curried
    >;

    // benchmarks registered in constructors for each type in tuple
    using instantiator_t = typename benchmarks_t::template apply<std::tuple>;
    instantiator_t{};

  }
};
