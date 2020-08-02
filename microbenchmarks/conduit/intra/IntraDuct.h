#include "../DuctMicrobenchUtils.h"

// sample doubling thread counts
ForEach<
  ThreadCountPayload,
  2
> range{};

// initialize
struct Initializer {
  Initializer() {
    range.item<1>();
  }
} i;

BENCHMARK_MAIN();
