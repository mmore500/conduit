#include "../DuctBenchmarkRegistration.h"
#include "utility/ScopeGuard.h"
#include "distributed/MPIGuard.h"

// sample doubling thread counts
uit::ForEach<
  ThreadCountPayload,
  128,
  2,
  std::multiplies<size_t>
> range{};

const uit::MPIGuard mguard;

const uit::ScopeGuard sguard{ [](){ range.item<1>(); } };

BENCHMARK_MAIN();
