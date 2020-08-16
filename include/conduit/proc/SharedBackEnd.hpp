#pragma once

#include "../../distributed/RDMAWindowManager.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class SharedBackEnd {

  uit::RDMAWindowManager window_manager;

public:

  uit::RDMAWindowManager & GetWindowManager() {
    return window_manager;
  }

  void Initialize() { window_manager.Initialize(); }

};

} // namespace uit
