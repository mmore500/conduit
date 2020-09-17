#include "uit/config/ImplSpec.hpp"
#include "uit/duct/mock/ThrowDuct.hpp"
#include "uit/duct/thread/put=dropping+get=stepping+type=any/a::BoundedMoodyCamelDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::BoundedMoodyCamelDuct,
  uit::a::BoundedMoodyCamelDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "../ValueThreadDuct.hpp"
#include "../SteppingThreadDuct.hpp"
