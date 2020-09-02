#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping/RingRingImsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::RingRingImsgDuct
>;

#include "../ProcDuct.hpp"
