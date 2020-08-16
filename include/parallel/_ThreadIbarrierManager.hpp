#pragma once

#include <memory>

#include "../containers/safe/deque.hpp"
#include "../polyfill/latch.hpp"

#include "ThreadIbarrier.hpp"
#include "ThreadMap.hpp"

namespace uit {
namespace internal {

class ThreadIbarrierManager
: public std::enable_shared_from_this<ThreadIbarrierManager>
{

  using latches_t = uit::safe::deque<internal::SharedLatch>;
  latches_t latches;

  uit::ThreadMap<latches_t::iterator> thread_positions;

  size_t expected;

  void Flush() {
    // try to discard old latches
    while (
      latches.size()
      && !latches.front().TryWait()
      && latches.front().IsReleased()
    ) latches.pop_front();
  }

public:

  ThreadIbarrierManager(const size_t expected_)
  : expected(expected_)
  { ; }

  uit::ThreadIbarrier MakeBarrier() {

    Flush();

    // race condition where multiple latches are appended is okay
    if (latches.empty()) latches.emplace_back(expected);

    auto& position = thread_positions.GetWithDefault(std::begin(latches));
    emp_assert(thread_positions.GetSize() <= expected);
    emp_assert(position != std::end(latches));

    return ThreadIbarrier{
      shared_from_this(),
      *(position++)
    };

  }

};

} // end namespace internal
} // end namespace uit
