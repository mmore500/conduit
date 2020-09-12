#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/thread/accumulating+type=any/a::MutexAccumulatingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::MutexAccumulatingDuct,
  uit::a::MutexAccumulatingDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "../AccumulatingThreadDuct.hpp"
