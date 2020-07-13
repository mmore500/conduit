#pragma once

#include <utility>
#include <variant>

#include "base/assert.h"
#include "tools/string_utils.h"

#include "MockDuct.h"
#include "ThreadDuct.h"
#include "ProcessInletDuct.h"
#include "ProcessOutletDuct.h"

#include "print_utils.h"

template<typename T, size_t N>
class Duct {

  std::variant<
    MockDuct<T, N>,
    ThreadDuct<T, N>,
    ProcessInletDuct<T, N>,
    ProcessOutletDuct<T, N>
  > impl;

public:

  template <typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    const size_t cur_pending = std::visit(
      [](auto & arg) { return arg.GetPending(); },
      impl
    );
    impl.template emplace<WhichDuct>(std::forward<Args>(args)...);
    std::visit(
      [cur_pending](auto & arg) { arg.pending = cur_pending; },
      impl
    );
  }

  //todo rename
  void Push() {

    std::visit(
      [](auto & arg) { arg.Push(); },
      impl
    );

  }

  //todo rename
  void Pop(const size_t count) {

    std::visit(
      [count](auto & arg) { arg.Pop(count); },
      impl
    );

  }

  size_t GetPending() const {
    return std::visit(
      [](auto & arg) -> size_t { return arg.GetPending(); },
      impl
    );
  }

  size_t GetAvailableCapacity() const {
    return std::visit(
      [](auto & arg) -> size_t { return arg.GetAvailableCapacity(); },
      impl
    );
  }

  // todo rename
  T GetElement(const size_t n) const {
    return std::visit(
      [n](auto & arg) -> T { return arg.GetElement(n); },
      impl
    );
  }

  void SetElement(const size_t n, const T & val) {
    std::visit(
      [n, &val](auto & arg) { arg.SetElement(n, val); },
      impl
    );
  }

  std::string ToString() const {
    return std::visit(
      [](auto & arg) -> std::string { return arg.ToString(); },
      impl
    );
  }


};
