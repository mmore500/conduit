#include <thread>

#include "mpi.h"

#include "concurrent/Gatherer.h"
#include "utility/CircularIndex.h"
#include "parallel/ThreadTeam.h"
#include "polyfill/barrier.h"


#include "distributed/mpi_utils.h"
#include "conduit/pipe_utils.h"
#include "utility/numeric_cast.h"

uit::Gatherer<int> gather(MPI_INT);

void do_work() {

  static std::barrier barrier{uit::numeric_cast<std::ptrdiff_t>(2)};

  barrier.arrive_and_wait();

  gather.Put(uit::get_thread_id());

}

int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  uit::ThreadTeam team;

  team.Add(do_work);
  team.Add(do_work);

  team.Join();

  auto res = gather.Gather();

  if (res) for (auto & val : *res) std::cout << "v: " << val << std::endl;

  MPI_Finalize();

  return 0;
}
