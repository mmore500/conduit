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

static void MPI_Irsend(benchmark::State& state) {

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

      if (uit::test_completion(requests.front())) {
        // if front request is complete, pop it
        requests.pop_front();
        buffers.pop_front();

        if (current_streak) ++streak_counter;
        current_streak=0;

      } else {
        // otherwise, log a drop try again
        ++current_streak;
        ++drop_counter;
        continue;
      }

    }

    // add a send request
    requests.emplace_back();
    buffers.emplace_back();
    UIT_Irsend(
      &buffers.back(), // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      1, // int dest
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &requests.back() // MPI_Request * request
    );

  }

  // log results
  state.counters.insert({
    {
      "Dropped Sends",
      benchmark::Counter(
        drop_counter
      )
    },
    {
      "Drop Rate",
      benchmark::Counter(
        drop_counter,
        benchmark::Counter::kIsRate
      )
    },
    {
      "Drop Fraction",
      benchmark::Counter(
        drop_counter / static_cast<double>(epoch_counter)
      )
    },
    {
      "Epochs",
      benchmark::Counter(
        epoch_counter
      )
    },
    {
      "Drop Streak Count",
      benchmark::Counter(
        streak_counter
      )
    },
    {
      "Average Drop Streak Length",
      benchmark::Counter(
        drop_counter / static_cast<double>(streak_counter)
      )
    },
    {
      "Processes",
      benchmark::Counter(
        uit::get_nprocs(),
        benchmark::Counter::kAvgThreads
      )
    }
  });

  // clean up
  // wait on all remaining send requests to complete
  emp::vector<MPI_Request> contiguous(std::begin(requests), std::end(requests));
  UIT_Waitall(
    contiguous.size(),
    contiguous.data(),
    MPI_STATUSES_IGNORE
  );

}

static void post_fresh_recvs(
  std::deque<MPI_Request>& requests,
  std::deque<int>& buffers
) {

  for (size_t i = 0; i < buffer_size; ++i) {
    requests.emplace_back();
    buffers.emplace_back();
    UIT_Irecv(
      &buffers.back(), // const void *buf
      1, // int count
      MPI_INT, // MPI_Datatype datatype
      0, // int source
      1, // int tag
      MPI_COMM_WORLD, // MPI_Comm comm
      &requests.back() // MPI_Request * request
    );
  }

  if (requests.size() > 2 * buffer_size) {

    // testing requests before releasing them is necessary for MPICH
    emp::vector<MPI_Request> contiguous(
      std::begin(requests),
      std::prev(std::end(requests), 2 * buffer_size)
    );
    UIT_Waitall(
      contiguous.size(),
      contiguous.data(),
      MPI_STATUSES_IGNORE
    );

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
  UIT_Barrier(MPI_COMM_WORLD);

  // this barrier will signal when benchmarking is complete
  MPI_Request ibarrier_request;
  UIT_Ibarrier(MPI_COMM_WORLD, &ibarrier_request);

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
      "MPI_Irsend",
      MPI_Irsend
    )
  );
}};

int main(int argc, char** argv) {

  // only root runs benchmark
  if (uit::is_root()) {

    benchmark::Initialize(&argc, argv);

    // wait for support to complete setup
    UIT_Barrier(MPI_COMM_WORLD);

    benchmark::RunSpecifiedBenchmarks();

    // notify support that benchmarking is complete
    MPI_Request ibarrier_request;
    UIT_Ibarrier(MPI_COMM_WORLD, &ibarrier_request);
    UIT_Wait(&ibarrier_request, MPI_STATUSES_IGNORE);

  } else {

    support();

  }


}
