#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/RdmaDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::RdmaDuct
>;

#include "ProcDuct.hpp"
