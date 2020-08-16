#pragma once

#include <atomic>
#include <deque>
#include <memory>
#include <shared_mutex>
#include <stddef.h>
#include <unordered_map>

#include "../../third-party/Empirical/source/base/assert.h"

#include "OncePerThreadChecker.hpp"
#include "ThreadMap.hpp"

namespace uit {

namespace internal {

class SharedLatch {

  size_t expected;

  size_t owner_count{};
  size_t arrival_count{};

  mutable std::shared_mutex mutex{};

  uit::OncePerThreadChecker checker;

public:

  SharedLatch(const size_t expected_)
  : expected(expected_)
  { ; }

  void AcquireAndArrive() {
    const std::unique_lock lock{ mutex };
    ++owner_count;
    ++arrival_count;
    checker.Check();
  }
  void Release() {
    const std::unique_lock lock{ mutex };
    emp_assert(owner_count);
    --owner_count;
  }
  bool TryWait() const {
    const std::shared_lock lock{ mutex };
    return arrival_count == expected;
  }
  bool IsObsolete() const {
    const std::shared_lock lock{ mutex };
    return owner_count == 0 && arrival_count == expected;
  }

};

class ThreadIbarrierManager;

} // end namespace internal

// TODO add occupancy caps
class ThreadIbarrier {

  friend class internal::ThreadIbarrierManager;

  std::shared_ptr<internal::ThreadIbarrierManager> manager;

  internal::SharedLatch& latch;


  ThreadIbarrier(
    std::shared_ptr<internal::ThreadIbarrierManager> manager_,
    internal::SharedLatch& latch_
  ) : manager(manager_)
  , latch(latch_)
  {
    emp_assert( !IsComplete() );
    latch.AcquireAndArrive();
  }

public:

  ~ThreadIbarrier() { latch.Release(); }

  bool IsComplete() const { return latch.TryWait(); }

};

} // end namespace uit
