#include "uit/setup/ImplSpec.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/thread/accumulating+type=fundamental/f::AtomicAccumulatingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::f::AtomicAccumulatingDuct,
  uit::f::AtomicAccumulatingDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "../AccumulatingThreadDuct.hpp"
