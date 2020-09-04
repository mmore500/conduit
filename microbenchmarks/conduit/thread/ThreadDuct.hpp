#include "uit/mpi/MpiGuard.hpp"
#include "uit/utility/ScopeGuard.hpp"

#include "../DuctBenchmarkRegistration.hpp"

// sample doubling thread counts
uit::ForEach<
  ThreadCountPayload,
  128,
  2,
  std::multiplies<size_t>
> range{};

const uit::MpiGuard mguard;

const uit::ScopeGuard sguard{ [](){ range.item<1>(); } };

BENCHMARK_MAIN();
