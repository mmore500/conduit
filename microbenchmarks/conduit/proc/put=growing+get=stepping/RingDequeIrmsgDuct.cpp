#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=growing+get=stepping/SteppingDequeIrmsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::SteppingDequeIrmsgDuct
>;

#include "../ProcDuct.hpp"
