#include "uit/config/ImplSpec.hpp"
#include "uit/ducts/thread/put=growing+get=skipping+type=any/a::MutexSconceDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::MutexSconceDuct
>;

#include "../ThreadDuct.hpp"
