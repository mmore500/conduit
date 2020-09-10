#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=growing+get=skipping/inlet=DequeIrsend+outlet=BlockIrecv_IdirObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IdirObiDuct
>;

#include "../ProcDuct.hpp"
