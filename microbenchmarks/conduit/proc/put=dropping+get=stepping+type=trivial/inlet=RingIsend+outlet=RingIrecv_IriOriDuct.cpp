#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping+type=trivial/inlet=RingIsend+outlet=RingIrecv_IriOriDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IriOriDuct
>;

#include "../ProcDuct.hpp"
