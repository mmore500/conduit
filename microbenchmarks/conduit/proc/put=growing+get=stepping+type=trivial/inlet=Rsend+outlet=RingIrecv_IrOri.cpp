#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=growing+get=stepping+type=trivial/inlet=Rsend+outlet=RingIrecv_IrOri.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IrOri
>;

#include "../ProcDuct.hpp"
