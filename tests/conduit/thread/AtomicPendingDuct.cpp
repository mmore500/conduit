#include "conduit/ImplSpec.hpp"
#include "conduit/mock/ThrowDuct.hpp"
#include "conduit/thread/AtomicPendingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::ThrowDuct
>;

#include "ThreadDuct.hpp"
