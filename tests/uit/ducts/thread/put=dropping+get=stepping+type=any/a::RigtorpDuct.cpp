#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/thread/put=dropping+get=stepping+type=any/a::RigtorpDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::RigtorpDuct,
  uit::a::RigtorpDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "../ValueThreadDuct.hpp"
#include "../SteppingThreadDuct.hpp"
