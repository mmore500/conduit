#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping+type=cereal/inlet=CerealringIrsend+outlet=CerealIprobe_IcrirOciDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IcrirOciDuct
>;

#include "../ProcDuct.hpp"
