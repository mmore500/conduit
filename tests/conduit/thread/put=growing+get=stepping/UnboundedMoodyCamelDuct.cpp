#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/thread/put=growing+get=stepping/UnboundedMoodyCamelDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::UnboundedMoodyCamelDuct,
  uit::UnboundedMoodyCamelDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "../ValueThreadDuct.hpp"
#include "../SteppingThreadDuct.hpp"
