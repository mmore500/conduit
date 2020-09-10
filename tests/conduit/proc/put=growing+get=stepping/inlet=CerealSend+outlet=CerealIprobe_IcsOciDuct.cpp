#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=stepping/inlet=CerealSend+outlet=CerealIprobe_IcsOciDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IcsOciDuct
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
