#pragma once

#include <sstream>
#include <string>

#include "uit/utility/CountdownTimer.hpp"
#include "uit/utility/CoarseClock.hpp"

#include "CellCollection.hpp"

class Job {

  CellCollection collection;

  uit::CountdownTimer<std::chrono::seconds, uit::CoarseClock> timer{
    std::chrono::seconds{20}
  };

  size_t iteration_counter{};

public:

  Job(const submesh_t& submesh)
  : collection(submesh) {
    while( !timer.IsComplete() ) {
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
