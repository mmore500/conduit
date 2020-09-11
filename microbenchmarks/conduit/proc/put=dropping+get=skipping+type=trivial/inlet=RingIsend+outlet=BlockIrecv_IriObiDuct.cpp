#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping+type=trivial/inlet=RingIsend+outlet=BlockIrecv_IriObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IriObiDuct
>;

#include "../ProcDuct.hpp"
