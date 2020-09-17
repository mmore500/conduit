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

#include "uitsl/chrono/TimeGuard.hpp"
#include "uitsl/debug/benchmark_utils.hpp"
#include "uitsl/debug/safe_cast.hpp"
#include "uitsl/mpi/mpi_utils.hpp"
#include "uitsl/math/stats_utils.hpp"
#include "uitsl/meta/Curry.hpp"
#include "uitsl/meta/ForEach.hpp"
#include "uitsl/nonce/CircularIndex.hpp"
#include "uitsl/parallel/ThreadTeam.hpp"
#include "uitsl/parallel/thread_utils.hpp"
#include "uitsl/polyfill/barrier.hpp"

#include "uit/fixtures/Conduit.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/mesh/Mesh.hpp"
#include "uit/topology/RingTopologyFactory.hpp"
#include "uit/topology/DyadicTopologyFactory.hpp"
#include "uit/topology/ProConTopologyFactory.hpp"


#include "DuctBenchmarkRunner.hpp"

using Spec = uit::ImplSpec<int, uit::DEFAULT_BUFFER, ImplSel>;

template<
  typename MeshFactory,
  typename NumThreadsType
>
struct RegisterBenchmarks {

  RegisterBenchmarks() {

    const std::string name{ emp::to_string(
      MeshFactory::GetName(),
      "/nodes:",
      NumThreadsType{}() * uitsl::get_nprocs()
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
    uitsl::report_confidence(res);

    // need to guarantee a fixed number of stop/starts for the
    // MPI barriers to sync up correctly
    if (uitsl::is_multiprocess()) res->Iterations(100 * std::kilo{}.num);
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
      uitsl::Curry<
        RegisterBenchmarks,
        uitsl::ValType<NumThreads>
      >::template curried
    >;

    // benchmarks registered in constructors for each type in tuple
    using instantiator_t = typename benchmarks_t::template apply<std::tuple>;
    instantiator_t{};

  }
};
