#include "conduit/ImplSpec.hpp"
#include "conduit/mock/ThrowDuct.hpp"
#include "conduit/thread/MoodyCamelDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::MoodyCamelDuct,
  uit::ThrowDuct
>;

#include "ThreadDuct.hpp"
