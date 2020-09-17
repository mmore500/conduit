#include "uit/config/ImplSpec.hpp"
#include "uit/duct/mock/ThrowDuct.hpp"
#include "uit/duct/thread/put=growing+get=skipping+type=any/a::MutexSconceDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::MutexSconceDuct,
  uit::a::MutexSconceDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "../ValueThreadDuct.hpp"
#include "../SkippingThreadDuct.hpp"
