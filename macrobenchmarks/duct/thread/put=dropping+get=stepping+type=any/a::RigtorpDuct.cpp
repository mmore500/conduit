#include "uit/config/ImplSpec.hpp"
#include "uit/duct/thread/put=dropping+get=stepping+type=any/a::RigtorpDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::RigtorpDuct
>;

#include "../ThreadDuct.hpp"
