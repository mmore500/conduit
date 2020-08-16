#pragma once

#include <atomic>
#include <deque>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "../../third-party/Empirical/source/base/assert.h"

#include "../polyfill/barrier.hpp"
#include "../polyfill/latch.hpp"

#include "OncePerThreadChecker.hpp"
#include "ThreadMap.hpp"

namespace uit {

namespace internal {

class SharedLatch {
  std::atomic<size_t> owner_count{};
  std::latch latch;
  uit::OncePerThreadChecker checker;

public:

  SharedLatch(const size_t expected)
  : latch(expected)
  { ; }

  void Acquire() { ++owner_count; }
  void Release() { emp_assert(owner_count.load()); --owner_count; }
  void Arrive() { latch.count_down(); checker.Check(); }
  bool TryWait() const { return latch.try_wait(); }
  bool IsHeld() const { return owner_count; }
  bool IsReleased() const { return !IsHeld(); }

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
    latch.Acquire();
    latch.Arrive();
  }

public:

  ~ThreadIbarrier() { latch.Release(); }

  bool IsComplete() const { return latch.TryWait(); }

};

} // end namespace uit
