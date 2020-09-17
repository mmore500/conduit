#include "uit/config/ImplSpec.hpp"
#include "uit/duct/thread/put=dropping+get=stepping+type=any/a::BoundedMoodyCamelDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::BoundedMoodyCamelDuct
>;

#include "../ThreadDuct.hpp"
