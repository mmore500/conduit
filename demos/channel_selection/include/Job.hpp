#pragma once

#include <fstream>
#include <limits>
#include <sstream>
#include <string>

#include "../third-party/Empirical/include/emp/io/NullStream.hpp"

#include "uitsl/chrono/CoarseClock.hpp"
#include "uitsl/chrono/ClockDeltaDetector.hpp"
#include "uitsl/concurrent/ConcurrentTimeoutBarrier.hpp"
#include "uitsl/countdown/ProgressBar.hpp"
#include "uitsl/countdown/Timer.hpp"
#include "uitsl/mpi/comm_utils.hpp"
#include "uitsl/parallel/ThreadIbarrierFactory.hpp"

#include "CellCollection.hpp"
#include "config/cfg.hpp"

class Job {

  CellCollection collection;

  using timer_t = uitsl::CoarseTimer;
  using bar_t = uitsl::ProgressBar<timer_t>;

  bar_t timer;

  size_t update_counter{};

public:

  Job(const size_t thread_idx, const submesh_t& submesh)
  : collection(submesh)
  , timer(
    uitsl::is_root() && thread_idx == 0 ? std::cout : emp::nout,
    cfg.RUN_SECONDS() ?: std::numeric_limits<double>::infinity()
  ) {

    // initialized first time thru the function,
    // so N_THREADS should be initialized
    static uitsl::ThreadIbarrierFactory factory{ cfg.N_THREADS() };

    // uitsl::ClockDeltaDetector inner_sync;
    uitsl::CoarseRealTimer inner_sync{ std::chrono::milliseconds{ 10 } };

    const bool use_intra = ( cfg.ASYNCHRONOUS() != 2 );
    const bool is_multiproc = uitsl::is_multiprocess();

    const uitsl::ConcurrentTimeoutBarrier<timer_t> barrier{
      factory.MakeBarrier(), timer
    };

    for ( const auto __ : timer ) {
      ++update_counter;
      collection.Update(use_intra);

      if ( !cfg.ASYNCHRONOUS() ) {
        const uitsl::ConcurrentTimeoutBarrier<timer_t> barrier{
          factory.MakeBarrier(), timer
        };
      } else if (
        is_multiproc
        && cfg.ASYNCHRONOUS() == 1
        && inner_sync.IsComplete() ) {
        const uitsl::ConcurrentTimeoutBarrier<timer_t> barrier{
          factory.MakeBarrier(), timer
        };
        inner_sync.Reset();
      }
    }

    std::ofstream( emp::keyname::pack({
      {"a", "updates_elapsed"},
      {"proc", emp::to_string( uitsl::get_proc_id() )},
      {"thread", emp::to_string( thread_idx )},
      {"ext", ".txt"},
    }) ) << update_counter << std::endl;;

    std::ofstream( emp::keyname::pack({
      {"a", "num_messages_sent"},
      {"proc", emp::to_string( uitsl::get_proc_id() )},
      {"thread", emp::to_string( thread_idx )},
      {"ext", ".txt"},
    }) ) << collection.GetNumMessagesSent() << std::endl;;

    std::ofstream( emp::keyname::pack({
      {"a", "num_messages_received"},
      {"proc", emp::to_string( uitsl::get_proc_id() )},
      {"thread", emp::to_string( thread_idx )},
      {"ext", ".txt"},
    }) ) << collection.GetNumMessagesReceived() << std::endl;;


    if ( cfg.ASYNCHRONOUS() ) {
      collection.PushOutputs();
      // try to get a consistent reading for num_conflicts
      const uitsl::ConcurrentTimeoutBarrier<timer_t> barrier{
        factory.MakeBarrier(),
        timer_t{ std::numeric_limits<double>::infinity() }
      };
    }

    collection.PullInputs();

    std::ofstream( emp::keyname::pack({
      {"a", "num_conflicts"},
      {"proc", emp::to_string( uitsl::get_proc_id() )},
      {"thread", emp::to_string( thread_idx )},
      {"ext", ".txt"},
    }) ) << collection.CountConflicts() << std::endl;;

  }

  std::string ToString() const {
    std::stringstream ss;
    ss << "job size " << collection.GetSize() << std::endl;
    ss << "updates elapsed " << update_counter << std::endl;
    ss << "num messages sent " << collection.GetNumMessagesSent() << std::endl;
    ss << "num messages received " << collection.GetNumMessagesReceived()
      << std::endl;
    ss << "num chanel conflicts " << collection.CountConflicts()
      << std::endl;
    return ss.str();
  }

};
