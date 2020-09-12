#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/thread/put=dropping+get=stepping+type=any/a::AtomicPendingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AtomicPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "../ValueThreadDuct.hpp"
#include "../SteppingThreadDuct.hpp"
