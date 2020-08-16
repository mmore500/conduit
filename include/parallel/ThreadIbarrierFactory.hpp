#pragma once

#include <stddef.h>
#include <memory>

#include "_ThreadIbarrierManager.hpp"
#include "ThreadIbarrier.hpp"

namespace uit {

class ThreadIbarrierFactory {

  std::shared_ptr<internal::ThreadIbarrierManager> manager;

public:

  ThreadIbarrierFactory(const size_t expected)
  : manager(std::make_shared<internal::ThreadIbarrierManager>(expected))
  { ; }

  uit::ThreadIbarrier MakeBarrier() { return manager->MakeBarrier(); }

};

}
