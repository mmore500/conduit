#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=growing+get=skipping/inlet=DequeIsend+outlet=BlockIrecv_IdiObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IdiObiDuct
>;

#include "../ProcDuct.hpp"
