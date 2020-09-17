#pragma once

#include <mutex>

#include "../../../third-party/Empirical/source/base/assert.h"

#include "thread_utils.hpp"

namespace uit {

class ThreadLocalChecker {

  uit::thread_id_t thread_id{ uit::get_thread_id() };

public:

  ThreadLocalChecker()
  { ; }

  void Check() { emp_assert( thread_id == uit::get_thread_id() ); }

};

} // namespace uit
