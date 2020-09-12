#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/put=dropping+get=stepping+type=any/a::BoundedMoodyCamelDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::BoundedMoodyCamelDuct
>;

#include "../ThreadDuct.hpp"
