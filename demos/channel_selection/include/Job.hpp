#pragma once

#include <limits>
#include <sstream>
#include <string>

#include "../third-party/Empirical/include/emp/io/NullStream.hpp"

#include "uitsl/chrono/CoarseClock.hpp"
#include "uitsl/concurrent/ConcurrentTimeoutBarrier.hpp"
#include "uitsl/countdown/ProgressBar.hpp"
#include "uitsl/countdown/Timer.hpp"
#include "uitsl/parallel/ThreadIbarrierFactory.hpp"

#include "CellCollection.hpp"
#include "config/cfg.hpp"

class Job {

  CellCollection collection;

  using timer_t = uitsl::CoarseTimer;
  using bar_t = uitsl::ProgressBar<timer_t>;

  bar_t timer;

  size_t iteration_counter{};

public:

  Job(const size_t thread_idx, const submesh_t& submesh)
  : collection(submesh)
  , timer(
    uitsl::is_root() && thread_idx == 0 ? std::cout : emp::nout,
    cfg.RUN_SECONDS() ?: std::numeric_limits<double>::infinity()
  ) {
    for ( const auto __ : timer ) {
      ++iteration_counter;
      collection.Update();

      if ( cfg.SYNCHRONOUS() ) {
        // initialized first time thru the function,
        // so N_THREADS should be initialized
        static uitsl::ThreadIbarrierFactory factory{ cfg.N_THREADS() };

        const uitsl::ConcurrentTimeoutBarrier<timer_t> barrier{
          factory.MakeBarrier(), timer
        };

      }

    }
  }

  std::string ToString() const {
    std::stringstream ss;
    ss << "job size " << collection.GetSize() << std::endl;
    ss << collection.ToString() << std::endl;
    ss << "iterations " << iteration_counter << std::endl;
    ss << "num messages sent " << collection.GetNumMessagesSent() << std::endl;
    ss << "num messages received " << collection.GetNumMessagesReceived()
      << std::endl;
    return ss.str();
  }

};
