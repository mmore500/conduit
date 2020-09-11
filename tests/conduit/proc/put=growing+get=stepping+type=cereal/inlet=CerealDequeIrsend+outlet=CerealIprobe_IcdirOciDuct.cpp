#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=stepping+type=cereal/inlet=CerealDequeIrsend+outlet=CerealIprobe_IcdirOciDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IcdirOciDuct
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
