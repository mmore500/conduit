#pragma once

#include <atomic>
#include <thread>

// TODO rename tid_t
using thread_id_t = size_t;

// TODO rename get_tid
thread_id_t get_thread_id() {
  static std::atomic<size_t> counter{};
  const thread_local size_t thread_id{counter++};
  return thread_id;
}
