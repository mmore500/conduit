#include "uit/config/ImplSpec.hpp"
#include "uit/duct/thread/accumulating+type=any/a::MutexAccumulatingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::MutexAccumulatingDuct
>;

#include "../ThreadDuct.hpp"
