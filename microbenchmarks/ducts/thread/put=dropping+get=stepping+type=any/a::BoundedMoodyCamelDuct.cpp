#include "uit/config/ImplSpec.hpp"
#include "uit/ducts/thread/put=dropping+get=stepping+type=any/a::BoundedMoodyCamelDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::BoundedMoodyCamelDuct
>;

#include "../ThreadDuct.hpp"
