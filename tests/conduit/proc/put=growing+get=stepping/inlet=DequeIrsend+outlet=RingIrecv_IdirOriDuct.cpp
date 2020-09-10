#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=stepping/inlet=DequeIrsend_outlet=RingIrecv_IdirOriDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IdirOriDuct
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
