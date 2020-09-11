#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/thread/put=growing+get=skipping+type=any/MutexSconceDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::MutexSconceDuct,
  uit::MutexSconceDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "../ValueThreadDuct.hpp"
#include "../SkippingThreadDuct.hpp"
