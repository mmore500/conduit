#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=stepping/CerealRingDequeIrmsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::CerealRingDequeIrmsgDuct
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
