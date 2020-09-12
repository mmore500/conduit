#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/put=growing+get=skipping+type=any/a::AtomicSconceDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicSconceDuct
>;

#include "../ThreadDuct.hpp"
