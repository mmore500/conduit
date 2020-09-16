#include <deque>

#include <mpi.h>
#include <benchmark/benchmark.h>

#include "uit/conduit/config.hpp"
#include "uit/mpi/MpiGuard.hpp"
#include "uit/mpi/mpi_utils.hpp"
#include "uit/debug/benchmark_utils.hpp"
#include "uit/nonce/ScopeGuard.hpp"

const uit::MpiGuard guard;

constexpr size_t buffer_size{ DEFAULT_BUFFER };

static void MPI_Test_nopending(benchmark::State& state) {

  // set up
  MPI_Request request;
  int buffer{};

  UIT_Irecv(
    &buffer, // const void *buf
    1, // int count
    MPI_INT, // MPI_Datatype datatype
    0, // int source
    1, // int tag
    MPI_COMM_WORLD, // MPI_Comm comm
    &request // MPI_Request * request
  );

  // benchmark
  for (auto _ : state) {

    int flag{};

    UIT_Test(
      &request,
      &flag,
      MPI_STATUS_IGNORE
    );

  }

  // log results
  state.counters.insert({
    {
      "Processes",
      benchmark::Counter(
        uit::get_nprocs(),
        benchmark::Counter::kAvgThreads
      )
    }
  });

  // clean up
  UIT_Cancel(&request);

}

static void MPI_Test_onepending(benchmark::State& state) {

  // set up
  MPI_Request request;
  int recv_buffer{};
  int send_buffer{};

  UIT_Irecv(
    &recv_buffer, // const void *buf
    1, // int count
    MPI_INT, // MPI_Datatype datatype
    0, // int source
    1, // int tag
    MPI_COMM_WORLD, // MPI_Comm comm
    &request // MPI_Request * request
  );

  UIT_Send(
    &send_buffer, // const void *buf
    1, // int count
    MPI_INT, // MPI_Datatype datatype
    0, // int dest
    1, // int tag
    MPI_COMM_WORLD // MPI_Comm comm
  );

  // benchmark
  for (auto _ : state) {

    int flag{};

    UIT_Test(
      &request,
      &flag,
      MPI_STATUS_IGNORE
    );

  }

  // clean up
  UIT_Wait(&request, MPI_STATUS_IGNORE);

}

static void MPI_Test_manypending(benchmark::State& state) {

  // set up
  MPI_Request request;
  int recv_buffer{};
  int send_buffer{};

  UIT_Irecv(
    &recv_buffer, // const void *buf
    1, // int count
    MPI_INT, // MPI_Datatype datatype
    0, // int source
    1, // int tag
    MPI_COMM_WORLD, // MPI_Comm comm
    &request // MPI_Request * request
  );

  for (size_t i = 0; i < DEFAULT_BUFFER; ++i) {
    MPI_Request send_request;
    UIT_Isend(
      &send_buffer, // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int dest
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &send_request // MPI_Request * request
    );
  }

  // benchmark
  for (auto _ : state) {

    int flag{};

    UIT_Test(
      &request,
      &flag,
      MPI_STATUS_IGNORE
    );

  }

  // clean up
  for (size_t i = 0; i < DEFAULT_BUFFER - 1; ++i) {
    UIT_Irecv(
      &recv_buffer, // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &request // MPI_Request * request
    );


    UIT_Wait(&request, MPI_STATUS_IGNORE);
  }

}

// register benchmarks
const uit::ScopeGuard registration{[](){
  uit::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Test_nopending",
      MPI_Test_nopending
    )
  );

  uit::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Test_onepending",
      MPI_Test_onepending
    )
  );

  uit::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Test_manypending",
      MPI_Test_manypending
    )
  );
}};

int main(int argc, char** argv) {

  // only root runs benchmark
  if (uit::is_root()) {

    benchmark::Initialize(&argc, argv);

    benchmark::RunSpecifiedBenchmarks();

  }

}
