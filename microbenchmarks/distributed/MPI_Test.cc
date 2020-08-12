#include <mpi.h>
#include <deque>

#include <benchmark/benchmark.h>

#include "distributed/mpi_utils.h"
#include "utility/ScopeGuard.h"
#include "distributed/MPIGuard.h"
#include "conduit/config.h"
#include "utility/benchmark_utils.h"

const uit::MPIGuard guard;

constexpr size_t buffer_size{ DEFAULT_BUFFER };

static void MPI_Test_nopending(benchmark::State& state) {

  // set up
  MPI_Request request;
  int buffer{};

  uit::verify(MPI_Irecv(
    &buffer, // const void *buf
    1, // int count
    MPI_INT, // MPI_Datatype datatype
    0, // int source
    1, // int tag
    MPI_COMM_WORLD, // MPI_Comm comm
    &request // MPI_Request * request
  ));

  // benchmark
  for (auto _ : state) {

    int flag{};

    uit::verify(MPI_Test(
      &request,
      &flag,
      MPI_STATUS_IGNORE
    ));

  }

  // clean up
  uit::verify(MPI_Cancel(&request));

}

static void MPI_Test_onepending(benchmark::State& state) {

  // set up
  MPI_Request request;
  int recv_buffer{};
  int send_buffer{};

  uit::verify(MPI_Irecv(
    &recv_buffer, // const void *buf
    1, // int count
    MPI_INT, // MPI_Datatype datatype
    0, // int source
    1, // int tag
    MPI_COMM_WORLD, // MPI_Comm comm
    &request // MPI_Request * request
  ));

  uit::verify(MPI_Send(
    &send_buffer, // const void *buf
    1, // int count
    MPI_INT, // MPI_Datatype datatype
    0, // int dest
    1, // int tag
    MPI_COMM_WORLD // MPI_Comm comm
  ));

  // benchmark
  for (auto _ : state) {

    int flag{};

    uit::verify(MPI_Test(
      &request,
      &flag,
      MPI_STATUS_IGNORE
    ));

  }

  // clean up
  uit::verify(MPI_Wait(&request, MPI_STATUS_IGNORE));

}

static void MPI_Test_manypending(benchmark::State& state) {

  // set up
  MPI_Request request;
  int recv_buffer{};
  int send_buffer{};

  uit::verify(MPI_Irecv(
    &recv_buffer, // const void *buf
    1, // int count
    MPI_INT, // MPI_Datatype datatype
    0, // int source
    1, // int tag
    MPI_COMM_WORLD, // MPI_Comm comm
    &request // MPI_Request * request
  ));

  for (size_t i = 0; i < DEFAULT_BUFFER; ++i) {
    MPI_Request send_request;
    uit::verify(MPI_Isend(
      &send_buffer, // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int dest
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &send_request // MPI_Request * request
    ));
  }

  // benchmark
  for (auto _ : state) {

    int flag{};

    uit::verify(MPI_Test(
      &request,
      &flag,
      MPI_STATUS_IGNORE
    ));

  }

  // clean up
  for (size_t i = 0; i < DEFAULT_BUFFER - 1; ++i) {
    uit::verify(MPI_Irecv(
      &recv_buffer, // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &request // MPI_Request * request
    ));


    uit::verify(MPI_Wait(&request, MPI_STATUS_IGNORE));
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
