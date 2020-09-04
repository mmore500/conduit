#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/thread/put=dropping+get=stepping/BoundedMoodyCamelDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::BoundedMoodyCamelDuct,
  uit::BoundedMoodyCamelDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "../ValueThreadDuct.hpp"
#include "../SteppingThreadDuct.hpp"
