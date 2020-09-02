#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=growing+get=skipping/BlockDequeIrmsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::BlockDequeIrmsgDuct
>;

#include "../ProcDuct.hpp"
