#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=skipping/BlockDequeImsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::BlockDequeImsgDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
