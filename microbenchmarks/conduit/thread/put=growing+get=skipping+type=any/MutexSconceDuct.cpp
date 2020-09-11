#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/put=growing+get=skipping+type=any/MutexSconceDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::MutexSconceDuct
>;

#include "../ThreadDuct.hpp"
