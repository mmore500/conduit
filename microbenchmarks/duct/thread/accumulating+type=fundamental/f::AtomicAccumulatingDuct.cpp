#include "uit/config/ImplSpec.hpp"
#include "uit/duct/thread/accumulating+type=fundamental/f::AtomicAccumulatingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::f::AtomicAccumulatingDuct
>;

#include "../ThreadDuct.hpp"
