#pragma once

#include "defaults.hpp"

namespace uit {

/**
 * Specifies the `Duct` implementations to be used for intra-thread, inter-
 * thread, and inter-process transmission.
 *
 * @tparam IntraDuct_ Implementation to use for intra-thread transmission.
 * @tparam ThreadDuct_ Implementation to use for inter-thread transmission.
 * @tparam ProcDuct_ Implementation to use for inter-process transmission
 */
template<
  template<typename> typename IntraDuct_ = uit::DefaultIntraDuct,
  template<typename> typename ThreadDuct_ = uit::DefaultThreadDuct,
  template<typename> typename ProcDuct_ = uit::DefaultProcDuct
>
struct ImplSelect {

  template<typename ImplSpec>
  using IntraDuct = IntraDuct_<ImplSpec>;

  template<typename ImplSpec>
  using ThreadDuct = ThreadDuct_<ImplSpec>;

  template<typename ImplSpec>
  using ProcDuct = ProcDuct_<ImplSpec>;

};

} // namespace uit
