#pragma once
#ifndef UITSL_PARALLEL_THREADLOCALCHECKER_HPP_INCLUDE
#define UITSL_PARALLEL_THREADLOCALCHECKER_HPP_INCLUDE

#include <cassert>
#include <mutex>

#include "thread_utils.hpp"

namespace uitsl {

class ThreadLocalChecker {

  uitsl::thread_id_t thread_id{ uitsl::get_thread_id() };

public:

  ThreadLocalChecker()
  { ; }

  void Check() { assert( thread_id == uitsl::get_thread_id() ); }

};

} // namespace uitsl

#endif // #ifndef UITSL_PARALLEL_THREADLOCALCHECKER_HPP_INCLUDE
