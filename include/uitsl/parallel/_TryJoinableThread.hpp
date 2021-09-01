#pragma once
#ifndef UITSL_PARALLEL__TRYJOINABLETHREAD_HPP_INCLUDE
#define UITSL_PARALLEL__TRYJOINABLETHREAD_HPP_INCLUDE

#include <chrono>
#include <future>
#include <thread>

namespace uitsl {
namespace internal {

// adapted from https://stackoverflow.com/a/17204246
class TryJoinableThread {

  std::packaged_task<void()> task;
  std::future<void> future;
  std::thread thread;

public:

  template <typename... Args>
  explicit TryJoinableThread(Args&&... args)
  : task( std::forward<Args>(args)... )
  , future( task.get_future() )
  , thread( std::move(task) )
  {}

  void Join() { thread.join(); }

  bool IsDone() const {
    using namespace std::chrono_literals;
    return future.wait_for(0s) == std::future_status::ready;
  }

  bool TryJoin() {
    if ( IsDone() ) {
      Join(); return true;
    } else return false;
  }

};

} // namespace internal
} // namespace uitsl

#endif // #ifndef UITSL_PARALLEL__TRYJOINABLETHREAD_HPP_INCLUDE
