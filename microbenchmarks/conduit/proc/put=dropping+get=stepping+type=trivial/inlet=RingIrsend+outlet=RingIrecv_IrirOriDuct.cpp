#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping+type=trivial/inlet=RingIrsend+outlet=RingIrecv_IrirOriDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IrirOriDuct
>;

#include "../ProcDuct.hpp"
