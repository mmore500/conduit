#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping/inlet=CerealRingIsend+outlet=CerealIprobe_IcriOci.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IcriOci
>;

#include "../ProcDuct.hpp"
