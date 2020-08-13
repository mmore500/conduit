#include "../DuctBenchmarkRegistration.hpp"
#include "utility/ScopeGuard.hpp"
#include "distributed/MPIGuard.hpp"

uit::ForEach<
  ThreadCountPayload,
  2
> range{};

const uit::MPIGuard mguard{};

const uit::ScopeGuard sguard{ [](){ range.item<1>(); } };

BENCHMARK_MAIN();
