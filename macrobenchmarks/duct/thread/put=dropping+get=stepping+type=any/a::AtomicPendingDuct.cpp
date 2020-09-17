#include "uit/config/ImplSpec.hpp"
#include "uit/duct/thread/put=dropping+get=stepping+type=any/a::AtomicPendingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct
>;

#include "../ThreadDuct.hpp"
