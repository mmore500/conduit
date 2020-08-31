#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=growing+get=stepping/SteppingRmsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::SteppingRmsgDuct
>;

#include "../ProcDuct.hpp"
