#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/put=growing+get=skipping+type=any/a::MutexSconceDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::MutexSconceDuct,
  uit::a::MutexSconceDuct
>;

#include "../ThreadDuct.hpp"
