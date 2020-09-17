#include "uit/config/ImplSpec.hpp"
#include "uit/duct/thread/put=growing+get=skipping+type=any/a::AtomicSconceDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicSconceDuct
>;

#include "../ThreadDuct.hpp"
