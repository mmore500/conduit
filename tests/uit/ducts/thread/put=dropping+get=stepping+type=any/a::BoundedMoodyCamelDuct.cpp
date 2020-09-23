#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/thread/put=dropping+get=stepping+type=any/a::BoundedMoodyCamelDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::BoundedMoodyCamelDuct,
  uit::a::BoundedMoodyCamelDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "../ValueThreadDuct.hpp"
#include "../SteppingThreadDuct.hpp"
