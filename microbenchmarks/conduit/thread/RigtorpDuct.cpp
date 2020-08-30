#include "conduit/ImplSpec.hpp"
#include "conduit/thread/RigtorpDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::RigtorpDuct
>;

#include "ThreadDuct.hpp"
