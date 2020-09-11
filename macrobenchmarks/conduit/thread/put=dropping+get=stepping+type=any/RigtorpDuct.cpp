#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/put=dropping+get=stepping+type=any/RigtorpDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::RigtorpDuct
>;

#include "../ThreadDuct.hpp"
