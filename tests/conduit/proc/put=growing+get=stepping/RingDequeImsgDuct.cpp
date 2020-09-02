#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=stepping/RingDequeImsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::RingDequeImsgDuct
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
