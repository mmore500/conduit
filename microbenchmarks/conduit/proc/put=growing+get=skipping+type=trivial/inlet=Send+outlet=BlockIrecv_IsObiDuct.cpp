#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=growing+get=skipping+type=trivial/inlet=Send+outlet=BlockIrecv_IsObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IsObiDuct
>;

#include "../ProcDuct.hpp"
