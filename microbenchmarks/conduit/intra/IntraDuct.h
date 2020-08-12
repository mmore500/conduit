#include "../DuctBenchmarkRegistration.h"
#include "utility/ScopeGuard.h"
#include "distributed/MPIGuard.h"

uit::ForEach<
  ThreadCountPayload,
  2
> range{};

const uit::MPIGuard mguard{};

const uit::ScopeGuard sguard{ [](){ range.item<1>(); } };

BENCHMARK_MAIN();
