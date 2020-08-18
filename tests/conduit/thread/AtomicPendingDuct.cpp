#include "conduit/ImplSpec.hpp"
#include "conduit/thread/AtomicPendingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::PendingDuct,
  uit::AtomicPendingDuct
>;

#include "ThreadDuct.hpp"
