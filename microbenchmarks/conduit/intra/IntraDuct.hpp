#include "uit/mpi/MpiGuard.hpp"
#include "uit/utility/ScopeGuard.hpp"

#include "../DuctBenchmarkRegistration.hpp"

uit::ForEach<
  ThreadCountPayload,
  2
> range{};

const uit::MpiGuard mguard{};

const uit::ScopeGuard sguard{ [](){ range.item<1>(); } };

BENCHMARK_MAIN();
