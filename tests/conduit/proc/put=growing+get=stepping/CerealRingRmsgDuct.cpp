#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=stepping/CerealRingRmsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::CerealRingRmsgDuct
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
