#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=stepping+type=cereal/inlet=CerealDequeIsend+outlet=CerealIprobe_IcdiOciDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IcdiOciDuct
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
