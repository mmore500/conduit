#include "conduit/ImplSpec.hpp"
#include "conduit/mock/ThrowDuct.hpp"
#include "conduit/thread/HeadTailDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::HeadTailDuct,
  uit::ThrowDuct,
  uit::ThrowDuct
>;

#include "IntraDuct.hpp"
