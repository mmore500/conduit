#pragma once

#include <mutex>
#include <unordered_set>
#include <limits>

#include "thread_utils.h"

class OccupancyCap {

  mutable std::mutex mutex;

  std::unordered_set<thread_id_t> occupants;

  const size_t maximum_occupancy;

public:

  OccupancyCap(
    const size_t maximum_occupancy_=std::numeric_limits<size_t>::max()
  ) : maximum_occupancy(maximum_occupancy_)
  { ; }

  void Enter() {

    const std::lock_guard guard{mutex};

    const thread_id_t id = get_thread_id();

    emp_assert(occupants.count(id) == 0);

    occupants.insert(id);

    //TODO write more descriptive error message
    emp_assert(occupants.size() <= maximum_occupancy);

  }

  size_t GetMaximumOccupancy() const { return maximum_occupancy; }


  void Exit() {

    const std::lock_guard guard{mutex};

    const thread_id_t id = get_thread_id();

    emp_assert(occupants.count(id));

    occupants.erase(id);

  }

};
