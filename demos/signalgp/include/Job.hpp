#pragma once

#include <sstream>
#include <string>

#include "../third-party/Empirical/source/tools/NullStream.h"

#include "uit/countdown/ProgressBar.hpp"
#include "uit/countdown/Timer.hpp"
#include "uit/utility/CoarseClock.hpp"

#include "CellCollection.hpp"

class Job {

  CellCollection collection;

  using timer_t = uit::Timer<std::chrono::seconds, uit::CoarseClock>;
  using bar_t = uit::ProgressBar<timer_t>;

  static inline emp::NullStream ns;

  bar_t timer{
    uit::is_root() ? std::cout : ns,
    std::chrono::seconds{ 20 }
  };

  size_t iteration_counter{};

public:

  Job(const submesh_t& submesh)
  : collection(submesh) {
    for ( auto __ : timer ) {
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
