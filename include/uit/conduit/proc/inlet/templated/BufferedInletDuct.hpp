#pragma once

#include <algorithm>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../third-party/Empirical/source/base/vector.h"
#include "../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../mpi/mpi_utils.hpp"
#include "../../../../mpi/Request.hpp"
#include "../../../../utility/CircularIndex.hpp"
#include "../../../../utility/identity.hpp"
#include "../../../../utility/print_utils.hpp"

#include "../../../InterProcAddress.hpp"

#include "impl/BufferSpec.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<
  template<typename> typename BackingDuct,
  typename ImplSpec
>
class BufferedInletDuct {

  using BufferSpec = uit::internal::BufferSpec<ImplSpec, BackingDuct>;

public:

  using BackEndImpl = typename BackingDuct<BufferSpec>::BackEndImpl;

private:

  using T = typename ImplSpec::T;
  constexpr inline static size_t B{ ImplSpec::B };

  using BackingInlet = typename BackingDuct<BufferSpec>::InletImpl;
  BackingInlet inlet;

  using buffer_t = typename BufferSpec::T;
  buffer_t buffer;

public:

  BufferedInletDuct(
    const uit::InterProcAddress& address,
    std::shared_ptr<BackEndImpl> back_end
  ) : inlet{address, back_end}
  { ; }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    if (buffer.size() < B) {
      buffer.push_back(val);
      return true;
    } else return false;
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  template<typename P>
  bool TryPut(P&& val) {
    if (buffer.size() < B) {
      buffer.push_back( std::forward<P>(val) );
      return true;
    } else return false;
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() {
    if ( buffer.size() ) {
      const bool res = inlet.TryPut( std::move(buffer) );
      buffer.clear();
      return res;
    }
    else return true;
  }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    throw "ConsumeGets called on BufferedInletDuct";
  }

  [[noreturn]] const T& Get() const {
    throw "Get called on BufferedInletDuct";
  }

  [[noreturn]] T& Get() {
    throw "Get called on BufferedInletDuct";
  }

  static std::string GetType() { return "BufferedInletDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    return ss.str();
  }

};

} // namespace uit
