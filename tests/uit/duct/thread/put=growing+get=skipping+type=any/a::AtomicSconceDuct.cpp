#include "uit/config/ImplSpec.hpp"
#include "uit/duct/mock/ThrowDuct.hpp"
#include "uit/duct/thread/put=growing+get=skipping+type=any/a::AtomicSconceDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AtomicSconceDuct,
  uit::a::AtomicSconceDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
// TODO FIXME fix segfault
// #include "../ValueThreadDuct.hpp"
// #include "../SkippingThreadDuct.hpp"
