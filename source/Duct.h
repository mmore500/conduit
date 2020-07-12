#pragma once

#include "base/assert.h"
#include "tools/string_utils.h"

#include "print_utils.h"

using pending_t = std::atomic<size_t>;

template<typename T, size_t N>
class Duct {

  using buffer_t = std::array<T, N>;

  pending_t pending{0};
  buffer_t buffer;

public:

  //todo rename
  void Push() {
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
  }

  const pending_t & GetPending() const { return pending; }

  buffer_t & GetBuffer() { return buffer; }

  const buffer_t & GetBuffer() const { return buffer; }

  std::string ToString() const {
    std::stringstream ss;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("pending_t pending", pending);
    return ss.str();
  }


};
