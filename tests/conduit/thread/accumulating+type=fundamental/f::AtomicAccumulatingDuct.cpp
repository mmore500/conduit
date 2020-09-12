#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/thread/accumulating+type=fundamental/f::AtomicAccumulatingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::f::AtomicAccumulatingDuct,
  uit::f::AtomicAccumulatingDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "../AccumulatingThreadDuct.hpp"
