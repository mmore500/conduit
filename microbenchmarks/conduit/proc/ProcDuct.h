#include "../DuctBenchmarkRegistration.h"
#include "utility/ScopeGuard.h"
#include "distributed/MPIGuard.h"

uit::ForEach<
  ThreadCountPayload,
  2
> range{};

// initialize
const uit::MPIGuard mpi_guard;
const uit::ScopeGuard register_benchmarks( [](){ range.item<1>(); } );

int main(int argc, char** argv) {

  // suppress json output for non-root procs
  benchmark::Initialize(&argc, argv);

  benchmark::RunSpecifiedBenchmarks();

}
