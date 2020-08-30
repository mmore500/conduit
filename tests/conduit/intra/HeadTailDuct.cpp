#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/intra/HeadTailDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::HeadTailDuct,
  uit::ThrowDuct,
  uit::ThrowDuct
>;

#include "IntraDuct.hpp"
