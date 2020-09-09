#pragma once

#include <stddef.h>

#include "../../../../third-party/Empirical/source/base/assert.h"

#include "../proc/backend/MockBackEnd.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class EmpAssertDuct {

  using T = typename ImplSpec::T;

public:

  using InletImpl = EmpAssertDuct<ImplSpec>;
  using OutletImpl = EmpAssertDuct<ImplSpec>;
  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

  /**
   * TODO.
   */
  template <typename... Args>
  EmpAssertDuct(Args&&... args) { ; }

  /**
   * TODO.
   *
   * @throws TODO.
   */
  [[noreturn]] bool TryPut(const T&) const {
    emp_assert(false, "TryPut called on EmpAssertDuct");
  }

  /**
   * TODO.
   *
   */
  [[noreturn]] void Flush() const {
    emp_assert(false, "Flush called on EmpAssertDuct");
  }

  /**
   * TODO.
   *
   * @throws TODO.
   */
  [[noreturn]] const T& Get() const {
    emp_assert(false, "Get called on EmpAssertDuct");
  }

  /**
   * TODO.
   *
   * @throws TODO.
   */
  [[noreturn]] T& Get() {
    emp_assert(false, "Get called on EmpAssertDuct");
  }

  /**
   * TODO.
   *
   * @throws TODO.
   */
  [[noreturn]] size_t TryConsumeGets(size_t) const {
    emp_assert(false, "ConsumeGets called on EmpAssertDuct");
  }

  /**
   * TODO.
   *
   * @returns TODO.
   */
  static std::string GetName() { return "EmpAssertDuct"; }

  /**
   * TODO.
   *
   * @returns TODO.
   */
  std::string ToString() const { return std::string{}; }

};

} // namespace uit
