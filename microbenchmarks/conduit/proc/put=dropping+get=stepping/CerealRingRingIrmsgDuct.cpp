#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping/CerealRingRingIrmsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::CerealRingRingIrmsgDuct
>;

#include "../ProcDuct.hpp"
