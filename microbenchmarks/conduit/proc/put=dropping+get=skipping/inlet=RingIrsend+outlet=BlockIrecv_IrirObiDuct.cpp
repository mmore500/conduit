#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping/inlet=RingIrsend+outlet=BlockIrecv_IrirObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IrirObiDuct
>;

#include "../ProcDuct.hpp"
