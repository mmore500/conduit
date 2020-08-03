#pragma once

#include <utility>
#include <variant>
#include <stddef.h>

#include "base/assert.h"
#include "tools/string_utils.h"
#include "meta/TypePack.h"

//TODO don't include these all here
#include "thread/AtomicPendingDuct.h"
#include "intra/PendingDuct.h"
#include "thread/HeadTailDuct.h"
#include "thread/SPSCQueueDuct.h"
#include "proc/ISendDuct.h"
#include "proc/IRecvDuct.h"
#include "proc/RputDuct.h"
#include "proc/WindowDuct.h"

#include "../utility/print_utils.h"
#include "config.h"
#include "../distributed/mpi_utils.h"

namespace uit {

template<typename T, size_t N=DEFAULT_BUFFER>
class Duct {

  using ducts_t = typename emp::TypePack<
    IntraDuct<T, N>,
    ThreadDuct<T, N>,
    ProcInletDuct<T, N>,
    ProcOutletDuct<T, N>
  >::make_unique;

  typename ducts_t::template apply<std::variant> impl;

public:

  template <typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    impl.template emplace<WhichDuct>(std::forward<Args>(args)...);
  }

  //todo refactor to remove this
  //keep write/read position *within* duct
  void Initialize(const size_t write_position) {

    std::visit(
      [=](auto & arg) { arg.Initialize(write_position); },
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

  size_t GetPending() {
    return std::visit(
      [](auto & arg) -> size_t { return arg.GetPending(); },
      impl
    );
  }

  size_t GetAvailableCapacity() {
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

  const void * GetPosition(const size_t n) const {
    return std::visit(
      [n](auto & arg) -> const void * { return arg.GetPosition(n); },
      impl
    );
  }

  void SetElement(const size_t n, const T & val) {
    std::visit(
      [n, &val](auto & arg) { arg.SetElement(n, val); },
      impl
    );
  }

  std::string GetType() const {
    return std::visit(
      [](auto & arg) -> std::string { return arg.GetType(); },
      impl
    );
  }


  std::string GetID() const {
    return emp::to_string(
      get_proc_id(),
      ":",
      this
    );
  }

  std::string ToString() const {
    std::stringstream ss;
    ss << format_member("GetID()", GetID()) << std::endl;
    ss << format_member(
      "std::variant impl",
      std::visit(
        [](auto & arg) -> std::string { return arg.ToString(); },
        impl
      )
    );
    return ss.str();
  }

};

}
