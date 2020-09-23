#pragma once

#include <mutex>

#include "../../../third-party/Empirical/source/base/assert.h"

#include "thread_utils.hpp"

namespace uitsl {

class ThreadLocalChecker {

  uitsl::thread_id_t thread_id{ uitsl::get_thread_id() };

public:

  ThreadLocalChecker()
  { ; }

  void Check() { emp_assert( thread_id == uitsl::get_thread_id() ); }

};

} // namespace uitsl
