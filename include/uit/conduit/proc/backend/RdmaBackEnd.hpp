#pragma once

#include "../../../distributed/RdmaWindowManager.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class RdmaBackEnd {

  uit::RdmaWindowManager window_manager;

public:

  uit::RdmaWindowManager& GetWindowManager() {
    return window_manager;
  }

  void Initialize() { window_manager.Initialize(); }

};

} // namespace uit
