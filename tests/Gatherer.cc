#include <thread>

#include "mpi.h"

#include "../source/Gatherer.h"
#include "../source/CircularIndex.h"
#include "../source/ThreadTeam.h"
#include "../source/barrier.h"


#include "../source/mpi_utils.h"
#include "../source/pipe_utils.h"
#include "../source/numeric_cast.h"

Gatherer<int> gather(MPI_INT);

void do_work() {

  static barrier barrier{numeric_cast<std::ptrdiff_t>(2)};

  barrier.arrive_and_wait();

  gather.Put(get_thread_id());

}

int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  ThreadTeam team;

  team.Add(do_work);
  team.Add(do_work);

  team.Join();

  auto res = gather.Gather();

  if (res) for (auto & val : *res) std::cout << "v: " << val << std::endl;

  MPI_Finalize();

  return 0;
}
