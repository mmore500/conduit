#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/thread/put=growing+get=skipping+type=any/AtomicSconceDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::AtomicSconceDuct,
  uit::AtomicSconceDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
// TODO FIXME fix segfault
// #include "../ValueThreadDuct.hpp"
// #include "../SkippingThreadDuct.hpp"
