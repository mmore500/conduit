#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/put=dropping+get=stepping/RigtorpDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::RigtorpDuct
>;

#include "../ThreadDuct.hpp"
