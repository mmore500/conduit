#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=growing+get=stepping+type=trivial/inlet=DequeIsend+outlet=RingIrecv_IdiOriDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IdiOriDuct
>;

#include "../ProcDuct.hpp"
