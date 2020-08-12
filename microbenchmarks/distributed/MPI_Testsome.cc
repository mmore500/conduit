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

static void MPI_Testsome_nopending(benchmark::State& state) {

  // set up
  emp::vector<MPI_Request> requests(DEFAULT_BUFFER);
  emp::vector<int> buffers(DEFAULT_BUFFER);

  for (size_t i = 0; i < DEFAULT_BUFFER; ++i) {
    uit::verify(MPI_Irecv(
      &buffers[i], // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &requests[i] // MPI_Request * request
    ));
  }

  // benchmark
  for (auto _ : state) {

    int flag{};

    thread_local std::array<int, DEFAULT_BUFFER> out_indices;

    uit::verify(MPI_Testsome(
      requests.size(), // int count
      requests.data(), // MPI_Request array_of_requests[]
      &flag, // int *outcount
      out_indices.data(), // int array_of_indices[]
      MPI_STATUSES_IGNORE // MPI_Status array_of_statuses[]
    ));

  }

  // clean up
  for (auto& request : requests)  uit::verify(MPI_Cancel(&request));

}

static void MPI_Testsome_onepending(benchmark::State& state) {

  // set up
  emp::vector<MPI_Request> recv_requests(DEFAULT_BUFFER);
  emp::vector<int> recv_buffers(DEFAULT_BUFFER);

  for (size_t i = 0; i < DEFAULT_BUFFER; ++i) {
    uit::verify(MPI_Irecv(
      &recv_buffers[i], // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &recv_requests[i] // MPI_Request * request
    ));
  }

  MPI_Request send_request;
  int send_buffer{};
  uit::verify(MPI_Isend(
    &send_buffer, // const void *buf
    1, // int count
    MPI_INT, // MPI_Datatype datatype
    0, // int dest
    1, // int tag
    MPI_COMM_WORLD, // MPI_Comm comm
    &send_request // MPI_Request * request
  ));
  uit::verify(MPI_Wait(&send_request, MPI_STATUS_IGNORE));

  // benchmark
  for (auto _ : state) {

    int flag{};

    thread_local std::array<int, DEFAULT_BUFFER> out_indices;

    uit::verify(MPI_Testsome(
      recv_requests.size(), // int count
      recv_requests.data(), // MPI_Request array_of_requests[]
      &flag, // int *outcount
      out_indices.data(), // int array_of_indices[]
      MPI_STATUSES_IGNORE // MPI_Status array_of_statuses[]
    ));

  }

  // clean up
  for (auto& request : recv_requests) {
    if (!uit::test_completion(request)) uit::verify(MPI_Cancel(&request));
  }

}

static void MPI_Testsome_somepending(benchmark::State& state) {

  // set up
  emp::vector<MPI_Request> recv_requests(DEFAULT_BUFFER);
  emp::vector<int> recv_buffers(DEFAULT_BUFFER);

  for (size_t i = 0; i < DEFAULT_BUFFER; ++i) {
    uit::verify(MPI_Irecv(
      &recv_buffers[i], // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &recv_requests[i] // MPI_Request * request
    ));
  }

  for (size_t i = 0; i < DEFAULT_BUFFER/2; ++i) {
    MPI_Request send_request;
    int send_buffer{};
    uit::verify(MPI_Isend(
      &send_buffer, // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int dest
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &send_request // MPI_Request * request
    ));
    uit::verify(MPI_Wait(&send_request, MPI_STATUS_IGNORE));
  }

  // benchmark
  for (auto _ : state) {

    int flag{};

    thread_local std::array<int, DEFAULT_BUFFER> out_indices;

    uit::verify(MPI_Testsome(
      recv_requests.size(), // int count
      recv_requests.data(), // MPI_Request array_of_requests[]
      &flag, // int *outcount
      out_indices.data(), // int array_of_indices[]
      MPI_STATUSES_IGNORE // MPI_Status array_of_statuses[]
    ));

  }

  // clean up
  for (auto& request : recv_requests) {
    if (!uit::test_completion(request)) uit::verify(MPI_Cancel(&request));
  }

}

static void MPI_Testsome_manypending(benchmark::State& state) {

  // set up
  emp::vector<MPI_Request> recv_requests(DEFAULT_BUFFER);
  emp::vector<int> recv_buffers(DEFAULT_BUFFER);

  for (size_t i = 0; i < DEFAULT_BUFFER; ++i) {
    uit::verify(MPI_Irecv(
      &recv_buffers[i], // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &recv_requests[i] // MPI_Request * request
    ));
  }

  for (size_t i = 0; i < DEFAULT_BUFFER; ++i) {
    MPI_Request send_request;
    int send_buffer{};
    uit::verify(MPI_Isend(
      &send_buffer, // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int dest
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &send_request // MPI_Request * request
    ));
    uit::verify(MPI_Wait(&send_request, MPI_STATUS_IGNORE));
  }

  // benchmark
  for (auto _ : state) {

    int flag{};

    thread_local std::array<int, DEFAULT_BUFFER> out_indices;

    uit::verify(MPI_Testsome(
      recv_requests.size(), // int count
      recv_requests.data(), // MPI_Request array_of_requests[]
      &flag, // int *outcount
      out_indices.data(), // int array_of_indices[]
      MPI_STATUSES_IGNORE // MPI_Status array_of_statuses[]
    ));

  }

  // clean up
  for (auto& request : recv_requests) {
    if (!uit::test_completion(request)) uit::verify(MPI_Cancel(&request));
  }

}

static void MPI_Testsome_manymanypending(benchmark::State& state) {

  // set up
  emp::vector<MPI_Request> recv_requests(DEFAULT_BUFFER);
  emp::vector<int> recv_buffers(DEFAULT_BUFFER);

  for (size_t i = 0; i < DEFAULT_BUFFER; ++i) {
    uit::verify(MPI_Irecv(
      &recv_buffers[i], // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &recv_requests[i] // MPI_Request * request
    ));
  }

  int send_buffer{};
  for (size_t i = 0; i < DEFAULT_BUFFER*2; ++i) {
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

    thread_local std::array<int, DEFAULT_BUFFER> out_indices;

    uit::verify(MPI_Testsome(
      recv_requests.size(), // int count
      recv_requests.data(), // MPI_Request array_of_requests[]
      &flag, // int *outcount
      out_indices.data(), // int array_of_indices[]
      MPI_STATUSES_IGNORE // MPI_Status array_of_statuses[]
    ));

  }

  // clean up
  for (auto& request : recv_requests) {
    if (!uit::test_completion(request)) uit::verify(MPI_Cancel(&request));
  }

  for (size_t i = 0; i < DEFAULT_BUFFER; ++i) {
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
    uit::verify(MPI_Wait(&request, MPI_STATUS_IGNORE));
  }

}

// register benchmarks
const uit::ScopeGuard registration{[](){

  uit::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Testsome_nopending",
      MPI_Testsome_nopending
    )
  );

  uit::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Testsome_onepending",
      MPI_Testsome_onepending
    )
  );

  uit::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Testsome_somepending",
      MPI_Testsome_somepending
    )
  );

  uit::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Testsome_manypending",
      MPI_Testsome_manypending
    )
  );

  uit::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Testsome_manymanypending",
      MPI_Testsome_manymanypending
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
