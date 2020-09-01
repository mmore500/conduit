#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/thread/put=growing+get=skipping/AtomicSconceDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicSconceDuct,
  uit::ThrowDuct
>;

// TODO flesh out stub test
int main(){ return 0; }
