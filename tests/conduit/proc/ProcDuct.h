#include <chrono>
#include <thread>

#include "mpi.h"

#include "conduit/config.h"
#include "utility/CircularIndex.h"
#include "distributed/mpi_utils.h"
#include "distributed/RDMAWindowManager.h"
#include "conduit/pipe_utils.h"
#include "utility/math_utils.h"

uit::Outlet<char> make_input() {

  auto input_pipe = uit::make_pipe<char>();
  auto & [__, outlet] = input_pipe;

  const int source = uit::circular_index(
    uit::get_rank(),
    uit::get_nprocs(),
    -1
  );

  outlet.SplitDuct<uit::ProcOutletDuct<char>>(uit::get_rank(), source);

  return outlet;

}

uit::Inlet<char> make_output() {

  // another way of doing it
  auto inlet = uit::make_sink<char>();

  const int dest = uit::circular_index(uit::get_rank(), uit::get_nprocs(), 1);

  inlet.EmplaceDuct<uit::ProcInletDuct<char>>(uit::get_rank(), dest);

  return inlet;

}


void run() {


  uit::Outlet<char> input = make_input();

  uit::Inlet<char> output = make_output();

  uit::RDMAWindowManager::Initialize();

  const char message = 65 + uit::get_rank();

  output.MaybePut(message);

  std::this_thread::sleep_for(std::chrono::seconds{1});

  char res;
  res = input.GetCurrent();

  std::cout << "rank " << uit::get_rank() << " sent " << message << std::endl;
  std::cout << "rank " << uit::get_rank() << " received " << res << std::endl;

}

int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  run();

  uit::RDMAWindowManager::Cleanup();

  MPI_Finalize();

  return 0;
}
