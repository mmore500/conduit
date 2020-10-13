#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/thread/put=growing+get=skipping+type=any/a::MutexSconceDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::MutexSconceDuct,
  uit::a::MutexSconceDuct,
  uit::ThrowDuct
>;

#include "../SkippingThreadDuct.hpp"
#include "../ThreadDuct.hpp"
#include "../ValueThreadDuct.hpp"
