#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/thread/put=dropping+get=stepping+type=any/a::RigtorpDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::RigtorpDuct,
  uit::a::RigtorpDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "../ValueThreadDuct.hpp"
#include "../SteppingThreadDuct.hpp"
