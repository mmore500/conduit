#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/put=dropping+get=stepping+type=any/BoundedMoodyCamelDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::BoundedMoodyCamelDuct
>;

#include "../ThreadDuct.hpp"
