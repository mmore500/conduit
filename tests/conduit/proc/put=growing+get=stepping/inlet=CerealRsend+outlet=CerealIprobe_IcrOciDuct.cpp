#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=stepping/inlet=CerealRsend+outlet=CerealIprobe_IcrOciDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IcrOciDuct
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
