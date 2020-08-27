#pragma once

#include <stddef.h>

#include "../../../third-party/Empirical/source/base/assert.h"

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

  /**
   * TODO.
   */
  template <typename... Args>
  EmpAssertDuct(Args&&... args) { ; }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  void Put(const T& val) { emp_assert(false, "Put called on EmpAssertDuct"); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  [[noreturn]] bool IsReadyForPut() {
    emp_assert(false, "IsReadyForPut called on EmpAssertDuct");
  }

  /**
  * TODO.
  *
  * @throws TODO.
   */
  [[noreturn]] const T& Get() {
    emp_assert(false, "Get called on EmpAssertDuct");
  }

  /**
   * TODO.
   *
   * @throws TODO.
   */
  [[noreturn]] size_t CountUnconsumedGets() {
    emp_assert(false, "CountUnconsumedGets called on EmpAssertDuct");
  }

  /**
   * TODO.
   *
   * @param n TODO.
   */
  [[noreturn]] void ConsumeGets(const size_t n) {
    emp_assert(false, "ConsumeGets called on EmpAssertDuct");
  }

  /**
   * TODO.
   */
  static std::string GetName() { return "EmpAssertDuct"; }

  /**
   * TODO.
   */
  std::string ToString() const { return std::string{}; }

};

} // namespace uit
