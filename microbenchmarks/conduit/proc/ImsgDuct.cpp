#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/ImsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::ImsgDuct
>;

#include "ProcDuct.hpp"
