#pragma once

#include <shared_mutex>

#include "thread_utils.hpp"

namespace uit {

class RecursiveExclusiveLock;

class RecursiveMutex {

  friend class uit::RecursiveExclusiveLock;

  std::shared_mutex mutex;
  uit::thread_id_t mutex_owner{ uit::max_thread };

  void RegisterThreadOwnership() {
    emp_assert(mutex_owner == uit::max_thread);
    mutex_owner = uit::get_thread_id();
  }

  void DeregisterThreadOwnership() {
    emp_assert(mutex_owner != uit::max_thread);
    mutex_owner = uit::max_thread;
  }

  bool HasThreadOwnership() const {
    return mutex_owner == uit::get_thread_id();
  }

public:

  std::shared_mutex& Mutex() { return mutex; }

};

}
