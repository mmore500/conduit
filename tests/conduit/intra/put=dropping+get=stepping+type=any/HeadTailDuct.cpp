#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/intra/put=dropping+get=stepping+type=any/HeadTailDuct.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::HeadTailDuct,
  uit::ThrowDuct,
  uit::ThrowDuct
>;

#include "../IntraDuct.hpp"
#include "../SteppingIntraDuct.hpp"
#include "../ValueIntraDuct.hpp"
