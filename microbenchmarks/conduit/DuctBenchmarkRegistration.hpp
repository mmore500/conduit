#pragma once

#include <iostream>
#include <list>
#include <thread>
#include <tuple>

#include <mpi.h>
#include <benchmark/benchmark.h>

#include "../../third-party/Empirical/source/meta/TypePack.h"
#include "../../third-party/Empirical/source/tools/Random.h"
#include "../../third-party/Empirical/source/tools/string_utils.h"

#include "uit/conduit/Conduit.hpp"
#include "uit/conduit/config.hpp"
#include "uit/conduit/ImplSpec.hpp"
#include "uit/mpi/mpi_utils.hpp"
#include "uit/mesh/Mesh.hpp"
#include "uit/topology/RingTopologyFactory.hpp"
#include "uit/topology/DyadicTopologyFactory.hpp"
#include "uit/topology/ProConTopologyFactory.hpp"
#include "uit/utility/benchmark_utils.hpp"
#include "uit/utility/CircularIndex.hpp"
#include "uit/utility/Curry.hpp"
#include "uit/utility/ForEach.hpp"
#include "uit/utility/numeric_cast.hpp"
#include "uit/utility/stats_utils.hpp"
#include "uit/utility/TimeGuard.hpp"
#include "uit/parallel/ThreadTeam.hpp"
#include "uit/parallel/thread_utils.hpp"
#include "uit/polyfill/barrier.hpp"

#include "DuctBenchmarkRunner.hpp"

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
