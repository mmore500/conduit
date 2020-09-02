#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=growing+get=skipping/BlockDequeImsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::BlockDequeImsgDuct
>;

#include "../ProcDuct.hpp"
