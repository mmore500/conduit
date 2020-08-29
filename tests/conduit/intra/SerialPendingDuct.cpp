#include "conduit/ImplSpec.hpp"
#include "conduit/intra/SerialPendingDuct.hpp"
#include "conduit/mock/ThrowDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::ThrowDuct
>;

#include "IntraDuct.hpp"
