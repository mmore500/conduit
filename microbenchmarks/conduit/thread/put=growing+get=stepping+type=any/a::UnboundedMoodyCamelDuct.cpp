#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/put=growing+get=stepping+type=any/a::UnboundedMoodyCamelDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::UnboundedMoodyCamelDuct
>;

#include "../ThreadDuct.hpp"
