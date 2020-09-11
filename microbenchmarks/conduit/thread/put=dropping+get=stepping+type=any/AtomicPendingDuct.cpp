#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/put=dropping+get=stepping+type=any/AtomicPendingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct
>;

#include "../ThreadDuct.hpp"
