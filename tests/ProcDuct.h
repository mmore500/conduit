#include <chrono>
#include <thread>

#include "mpi.h"

#include "conduit/config.h"
#include "utility/CircularIndex.h"
#include "distributed/mpi_utils.h"
#include "conduit/pipe_utils.h"
#include "utility/math_utils.h"

Outlet<char> make_input() {

  auto input_pipe = make_pipe<char>();
  auto & [__, outlet] = input_pipe;

  const int source = circular_index(get_rank(), get_nprocs(), -1);

  outlet.SplitDuct<ProcOutletDuct<char>>(get_rank(), source);

  return outlet;

}

Inlet<char> make_output() {

  // another way of doing it
  auto inlet = make_sink<char>();

  const int dest = circular_index(get_rank(), get_nprocs(), 1);

  inlet.EmplaceDuct<ProcInletDuct<char>>(get_rank(), dest);

  return inlet;

}


int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  Outlet<char> input = make_input();

  Inlet<char> output = make_output();

  RDMAWindow::Initialize();

  const char message = 65 + get_rank();

  output.MaybePut(message);

  std::this_thread::sleep_for(std::chrono::seconds{1});

  char res;
  res = input.GetCurrent();

  std::cout << "rank " << get_rank() << " sent " << message << std::endl;
  std::cout << "rank " << get_rank() << " received " << res << std::endl;

  MPI_Finalize();

  return 0;
}
