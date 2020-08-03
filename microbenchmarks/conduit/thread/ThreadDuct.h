#include <thread>
#include <tuple>
#include <benchmark/benchmark.h>

#include "mpi.h"

#include "meta/TypePack.h"
#include "tools/Random.h"

#include "conduit/config.h"
#include "utility/CircularIndex.h"
#include "parallel/ThreadTeam.h"
#include "mesh/Mesh.h"
#include "utility/TimeGuard.h"
#include "utility/ForEach.h"
#include "utility/Curry.h"

#include "distributed/mpi_utils.h"
#include "conduit/pipe_utils.h"
#include "utility/numeric_cast.h"
#include "mesh/mesh_utils.h"
#include "parallel/thread_utils.h"
#include "utility/benchmark_utils.h"
#include "utility/stats_utils.h"

#include "../DuctMicrobenchRunner.h"
#include "../DuctMicrobenchUtils.h"

// sample doubling thread counts
uit::ForEach<
  ThreadCountPayload,
  128,
  2,
  std::multiplies<size_t>
> range{};

// initialize
struct Initializer {
  Initializer() {
    range.item<1>();
  }
} i;

BENCHMARK_MAIN();
