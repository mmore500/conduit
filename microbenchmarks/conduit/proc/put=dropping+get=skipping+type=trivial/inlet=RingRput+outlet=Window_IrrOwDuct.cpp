#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping+type=trivial/inlet=RingRput+outlet=Window_IrrOwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IrrOwDuct
>;

#include "../ProcDuct.hpp"
