#pragma once

#include <stddef.h>

#include "../proc/backend/MockBackEnd.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class ThrowDuct {

  using T = typename ImplSpec::T;

public:

  using InletImpl = ThrowDuct<ImplSpec>;
  using OutletImpl = ThrowDuct<ImplSpec>;
  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

  /**
   * TODO.
   */
  template <typename... Args>
  ThrowDuct(Args&&... args) { ; }

  /**
   * TODO.
   *
   */
  void Put(const T&) const { throw "Put called on ThrowDuct"; }

  /**
   * TODO.
   *
   * @throws TODO.
   */
  [[noreturn]] bool IsReadyForPut() const {
    throw "IsReadyForPut called on ThrowDuct";
  }

  /**
  * TODO.
  *
  * @throws TODO.
   */
  [[noreturn]] const T& Get() const { throw "Get called on ThrowDuct"; }

  /**
   * TODO.
   *
   * @throws TODO.
   */
  [[noreturn]] size_t TryConsumeGets(size_t) const {
    throw "ConsumeGets called on ThrowDuct";
  }

  /**
   * TODO.
   */
  static std::string GetName() { return "ThrowDuct"; }

  /**
   * TODO.
   */
  std::string ToString() const { return std::string{}; }

};

} // namespace uit
