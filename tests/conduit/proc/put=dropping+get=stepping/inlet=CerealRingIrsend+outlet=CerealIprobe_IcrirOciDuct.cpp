#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping/inlet=CerealRingIsend+outlet=CerealIprobe_IcriOciDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IcrirOciDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
