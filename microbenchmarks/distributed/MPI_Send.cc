#include <mpi.h>
#include <deque>

#include <benchmark/benchmark.h>

#include "distributed/mpi_utils.h"
#include "distributed/MPIGuard.h"
#include "conduit/config.h"
#include "utility/ScopeGuard.h"
#include "utility/benchmark_utils.h"

const uit::MPIGuard guard;

constexpr size_t buffer_size{ DEFAULT_BUFFER };

static void MPI_Send(benchmark::State& state) {

  // set up
  std::deque<MPI_Request> requests;
  std::deque<int> buffers;
  size_t drop_counter{};
  size_t streak_counter{};
  size_t current_streak{};
  size_t epoch_counter{};


  // benchmark
  for (auto _ : state) {

    ++epoch_counter;

    // if send buffer is at capacity, make some space
    if (requests.size() == buffer_size) {

      // if front request is complete, pop it
      requests.pop_front();
      buffers.pop_front();

    }

    // add a send request
    requests.emplace_back();
    buffers.emplace_back();
    uit::verify(MPI_Send(
      &buffers.back(), // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      1, // int dest
      1, // int tag
      MPI_COMM_WORLD // MPI_Comm comm
    ));

  }

}

static void post_fresh_recvs(
  std::deque<MPI_Request>& requests,
  std::deque<int>& buffers
) {

  for (size_t i = 0; i < buffer_size; ++i) {
    requests.emplace_back();
    buffers.emplace_back();
    uit::verify(MPI_Irecv(
      &buffers.back(), // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &requests.back() // MPI_Request * request
    ));
  }

  if (requests.size() > 2 * buffer_size) {

    // testing requests before releasing them is necessary for MPICH
    emp::vector<MPI_Request> contiguous(
      std::begin(requests),
      std::prev(std::end(requests), 2 * buffer_size)
    );
    uit::verify(MPI_Waitall(
      contiguous.size(),
      contiguous.data(),
      MPI_STATUSES_IGNORE
    ));

    requests.erase(
      std::begin(requests),
      std::prev(std::end(requests), 2 * buffer_size)
    );

  }


}


// post continuous stream of receives to match incoming sends
static void support() {

  std::deque<MPI_Request> requests;
  std::deque<int> buffers;

  post_fresh_recvs(requests, buffers);

  // signal setup is complete
  uit::verify(MPI_Barrier(MPI_COMM_WORLD));

  // this barrier will signal when benchmarking is complete
  MPI_Request ibarrier_request;
  uit::verify(MPI_Ibarrier(MPI_COMM_WORLD, &ibarrier_request));

  // loop until benchmarking is complete
  while (!uit::test_completion(ibarrier_request)) {

    // has sender started to catch up with our posted recv's?
    if (uit::test_completion(requests[requests.size() - buffer_size])) {
      post_fresh_recvs(requests, buffers);
    }

  }

}

// register benchmark
const uit::ScopeGuard registration{[](){
  uit::report_confidence(
    benchmark::RegisterBenchmark(
      "MPI_Send",
      MPI_Send
    )
  );
}};

int main(int argc, char** argv) {

  // only root runs benchmark
  if (uit::is_root()) {

    benchmark::Initialize(&argc, argv);

    // wait for support to complete setup
    uit::verify(MPI_Barrier(MPI_COMM_WORLD));

    benchmark::RunSpecifiedBenchmarks();

    // notify support that benchmarking is complete
    MPI_Request ibarrier_request;
    uit::verify(MPI_Ibarrier(MPI_COMM_WORLD, &ibarrier_request));

  } else {

    support();

  }


}
