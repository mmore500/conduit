#pragma once

#include <iterator>
#include <memory>
#include <mutex>

#include "../containers/safe/list.hpp"
#include "../polyfill/latch.hpp"

#include "ThreadIbarrier.hpp"
#include "ThreadMap.hpp"

namespace uit {
namespace internal {

class ThreadIbarrierManager
: public std::enable_shared_from_this<ThreadIbarrierManager>
{

  using latches_t = uit::safe::list<internal::SharedLatch>;
  latches_t latches;

  uit::ThreadMap<latches_t::iterator> thread_positions;

  size_t expected;

  std::mutex flush_mutex;

  void Flush() {

    // prevent double-popping race condition
    const std::lock_guard guard{ flush_mutex };

    // try to discard old latches
    while ( latches.size() && latches.front().IsObsolete() ) {
      latches.pop_front();
    }

  }

public:

  ThreadIbarrierManager(const size_t expected_)
  : expected(expected_)
  { ; }

  uit::ThreadIbarrier MakeBarrier() {

    // race condition where multiple latches are appended is okay
    if (latches.empty()) latches.emplace_back(expected);

    auto& position = thread_positions.GetWithDefault(std::begin(latches));
    emp_assert(thread_positions.GetSize() <= expected);

    emp_assert(position != std::end(latches));
    emp_assert(!position->TryWait());

    // race condition where multiple latches are appended is okay
    if (std::next(position) == std::end(latches)) {
      latches.emplace_back(expected);
      emp_assert(std::next(position) != std::end(latches));
    }

    std::advance(position, 1);
    emp_assert(position != std::end(latches));
    emp_assert(!position->TryWait());

    Flush();

    return ThreadIbarrier{
      shared_from_this(),
      *std::prev(position)
    };

  }

};

} // end namespace internal
} // end namespace uit
