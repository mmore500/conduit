#pragma once

#include "../../distributed/RDMAWindowManager.h"

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

};

}
