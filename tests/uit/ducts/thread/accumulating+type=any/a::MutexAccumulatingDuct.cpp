#include "uit/setup/ImplSpec.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/thread/accumulating+type=any/a::MutexAccumulatingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::MutexAccumulatingDuct,
  uit::a::MutexAccumulatingDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "../AccumulatingThreadDuct.hpp"
