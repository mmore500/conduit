#pragma once

#include "../third-party/SPSCQueue/include/rigtorp/SPSCQueue.h"

#include "base/assert.h"
#include "tools/string_utils.h"

#include "../config.h"
#include "../../utility/print_utils.h"

namespace uit {

template<typename T, size_t N>
class Duct;

template<typename T, size_t N=DEFAULT_BUFFER>
class SPSCQueueDuct {

  friend Duct<T, N>;

  using pending_t = std::atomic<size_t>;
  using buffer_t = std::array<T, N>;

  pending_t pending{0};
  rigtorp::SPSCQueue<T> queue{N};

public:

  void Initialize(const size_t write_position) { ; }

  //todo rename
  void Push() {

    emp_assert(
      pending < N,
      [](){ error_message_mutex.lock(); return "locked"; }(),
      emp::to_string("pending: ", pending)
    );

    pending.fetch_add(1, std::memory_order_relaxed);
  }

  //todo rename
  void Pop(const size_t count) {

    emp_assert(
      pending >= count,
      [](){ error_message_mutex.lock(); return "locked"; }(),
      emp::to_string("pending: ", pending),
      emp::to_string("count: ", count)
    );

    pending.fetch_sub(count, std::memory_order_relaxed);
    queue.pop();

  }

  size_t GetPending() { return pending; }

  size_t GetAvailableCapacity() { return N - pending; }

  T GetElement(const size_t n) const {
    const auto res = queue.front();
    return res ? *res : T{};
  }

  const void * GetPosition(const size_t n) const { return nullptr; }

  void SetElement(const size_t n, const T & val) {
    queue.try_push(val);
  }

  std::string GetType() const { return "SPSCQueueDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
      // ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("pending_t pending", (size_t) pending);
    return ss.str();
  }


};

}
