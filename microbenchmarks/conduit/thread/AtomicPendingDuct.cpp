#include "conduit/ImplSpec.hpp"
#include "conduit/thread/AtomicPendingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct
>;

#include "ThreadDuct.hpp"
