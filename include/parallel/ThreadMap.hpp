#pragma once

#include <mutex>
#include <unordered_map>
#include <shared_mutex>

#include "../containers/safe/unordered_map.hpp"

#include "thread_utils.hpp"

namespace uit {

template<typename T>
class ThreadMap {

  uit::safe::unordered_map<uit::thread_id_t, T> map;

public:

  T& GetWithDefault(const T& default_=T{}) {
    const uit::thread_id_t thread_id{ uit::get_thread_id() };

    if (map.count(thread_id) == 0) map.emplace(thread_id, default_);

    return map.at(thread_id);
  }

  size_t GetSize() const { return map.size(); }

};

}
