#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=growing+get=skipping+type=trivial/inlet=Rsend+outlet=BlockIrecv_IrObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IrObiDuct
>;

#include "../ProcDuct.hpp"
