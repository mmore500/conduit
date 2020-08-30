#include "conduit/ImplSpec.hpp"
#include "conduit/proc/RdmaDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::RdmaDuct
>;

#include "ProcDuct.hpp"
