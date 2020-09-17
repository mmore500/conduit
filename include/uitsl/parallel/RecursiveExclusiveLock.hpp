#pragma once

#include <shared_mutex>

#include "../../../third-party/Empirical/source/base/optional.h"

#include "RecursiveMutex.hpp"
#include "thread_utils.hpp"

namespace uit {

class RecursiveExclusiveLock {

  uit::RecursiveMutex& mutex;
  emp::optional<std::unique_lock<std::shared_mutex>> lock;

public:

  RecursiveExclusiveLock(uit::RecursiveMutex& mutex_)
  : mutex(mutex_)
  {
    if (not mutex.HasThreadOwnership()) {
      lock.emplace(mutex.Mutex());
      mutex.RegisterThreadOwnership();
    }
  }

  ~RecursiveExclusiveLock() { if (lock) mutex.DeregisterThreadOwnership(); }

};

} // namespace uit
