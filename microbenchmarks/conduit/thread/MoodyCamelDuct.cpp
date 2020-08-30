#include "conduit/ImplSpec.hpp"
#include "conduit/thread/MoodyCamelDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::MoodyCamelDuct
>;

#include "ThreadDuct.hpp"
