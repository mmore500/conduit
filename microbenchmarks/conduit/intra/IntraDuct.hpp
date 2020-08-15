#include "distributed/MPIGuard.hpp"
#include "utility/ScopeGuard.hpp"

#include "../DuctBenchmarkRegistration.hpp"

uit::ForEach<
  ThreadCountPayload,
  2
> range{};

const uit::MPIGuard mguard{};

const uit::ScopeGuard sguard{ [](){ range.item<1>(); } };

BENCHMARK_MAIN();
