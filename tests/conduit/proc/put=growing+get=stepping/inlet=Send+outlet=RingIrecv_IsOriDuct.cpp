#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=stepping/inlet=DequeIrsend+outlet=RingIrecv_IdirOriDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IsOriDuct
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
