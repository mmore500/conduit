#pragma once

#include <atomic>
#include <thread>

void get_thread_id() {
  static std::atomic<size_t> counter{};
  const thread_local size_t thread_id{counter++};
  return thread_id;
}
