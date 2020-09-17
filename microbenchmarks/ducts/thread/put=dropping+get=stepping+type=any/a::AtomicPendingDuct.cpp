#include "uit/setup/ImplSpec.hpp"
#include "uit/ducts/thread/put=dropping+get=stepping+type=any/a::AtomicPendingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct
>;

#include "../ThreadDuct.hpp"
