#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/put=growing+get=skipping/AtomicSconceDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::AtomicSconceDuct
>;

#include "../ThreadDuct.hpp"
