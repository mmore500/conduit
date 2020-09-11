#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=growing+get=stepping+type=cereal/inlet=CerealRsend+outlet=CerealIprobe_IcrOciDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IcdirOciDuct
>;

#include "../ProcDuct.hpp"
