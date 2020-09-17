#include "uit/config/ImplSpec.hpp"
#include "uit/duct/mock/ThrowDuct.hpp"
#include "uit/duct/thread/put=growing+get=stepping+type=any/a::UnboundedMoodyCamelDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::UnboundedMoodyCamelDuct,
  uit::a::UnboundedMoodyCamelDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "../ValueThreadDuct.hpp"
#include "../SteppingThreadDuct.hpp"
