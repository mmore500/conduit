#include <chrono>
#include <thread>

#include "mpi.h"

#include "../source/CircularIndex.h"
#include "../source/mpi_utils.h"
#include "../source/pipe_utils.h"

Outlet<char> make_input() {

  auto input_pipe = make_pipe<char>();
  auto & [__, outlet] = input_pipe;

  const int source = circular_index(get_rank(), get_nprocs(), -1);
  const int dest = get_rank();

  outlet.EmplaceDuct<ProcessOutletDuct<char>>(
    MPI_COMM_WORLD,
    source,
    dest,
    0
  );

  return outlet;

}

Inlet<char> make_output() {

  auto output_pipe = make_pipe<char>();
  auto & [inlet, __] = output_pipe;

  const int source = get_rank();
  const int dest = circular_index(get_rank(), get_nprocs(), 1);

  inlet.EmplaceDuct<ProcessInletDuct<char>>(
    MPI_COMM_WORLD,
    source,
    dest,
    0
  );

  return inlet;

}


int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  Inlet<char> output = make_output();

  Outlet<char> input = make_input();

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
