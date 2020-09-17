#include "uit/config/ImplSpec.hpp"
#include "uit/duct/mock/ThrowDuct.hpp"
#include "uit/duct/thread/put=dropping+get=stepping+type=any/a::RigtorpDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::RigtorpDuct,
  uit::a::RigtorpDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "../ValueThreadDuct.hpp"
#include "../SteppingThreadDuct.hpp"
