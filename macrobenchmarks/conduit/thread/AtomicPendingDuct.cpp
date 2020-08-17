#include "conduit/ImplSpec.hpp"
#include "conduit/thread/AtomicPendingDuct.hpp"

using ImplSel = uit::ImplSelector<
  uit::PendingDuct,
  uit::AtomicPendingDuct
>;

#include "ThreadDuct.hpp"
