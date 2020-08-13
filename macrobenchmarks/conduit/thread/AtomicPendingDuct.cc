#include "conduit/ImplSpec.h"
#include "conduit/thread/AtomicPendingDuct.h"

using ImplSel = uit::ImplSelector<
  uit::PendingDuct,
  uit::AtomicPendingDuct
>;

#include "ThreadDuct.h"
