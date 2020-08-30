#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/thread/MoodyCamelDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::MoodyCamelDuct,
  uit::ThrowDuct
>;

#include "ThreadDuct.hpp"
