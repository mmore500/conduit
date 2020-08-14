#pragma once

#include "../../distributed/RDMAWindowManager.hpp"

namespace uit {

// name ideas: support, kernel, state, info, backing, resources,
// shared resources, joint, mutual, pool, backend, backing

template<typename ImplSpec>
class SharedBackEnd {

  uit::RDMAWindowManager window_manager;

public:

  uit::RDMAWindowManager & GetWindowManager() {
    return window_manager;
  }

  void Initialize() { window_manager.Initialize(); }

};

}
