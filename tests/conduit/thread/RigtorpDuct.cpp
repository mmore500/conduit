#include "conduit/ImplSpec.hpp"
#include "conduit/mock/ThrowDuct.hpp"
#include "conduit/thread/RigtorpDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::RigtorpDuct,
  uit::ThrowDuct
>;

#include "ThreadDuct.hpp"
