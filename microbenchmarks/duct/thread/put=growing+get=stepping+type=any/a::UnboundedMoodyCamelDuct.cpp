#include "uit/config/ImplSpec.hpp"
#include "uit/duct/thread/put=growing+get=stepping+type=any/a::UnboundedMoodyCamelDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::UnboundedMoodyCamelDuct
>;

#include "../ThreadDuct.hpp"
