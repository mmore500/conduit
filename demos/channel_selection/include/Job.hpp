#pragma once

#include <sstream>
#include <string>

#include "../third-party/Empirical/include/emp/io/NullStream.hpp"

#include "uitsl/chrono/CoarseClock.hpp"
#include "uitsl/countdown/ProgressBar.hpp"
#include "uitsl/countdown/Timer.hpp"

#include "CellCollection.hpp"

class Job {

  CellCollection collection;

  using timer_t = uitsl::Timer<std::chrono::seconds, uitsl::CoarseClock>;
  using bar_t = uitsl::ProgressBar<timer_t>;

  bar_t timer{
    uitsl::is_root() ? std::cout : emp::nout,
    std::chrono::seconds{ 20 }
  };

  size_t iteration_counter{};

public:

  Job(const submesh_t& submesh, const size_t num_nodes)
  : collection(submesh, num_nodes) {
    for ( const auto __ : timer ) {
      ++iteration_counter;
      collection.Update();
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
