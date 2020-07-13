#include <chrono>
#include <thread>

#include "mpi.h"

#include "../source/mpi_utils.h"
#include "../source/pipe_utils.h"

// TODO make a util
// TODO actually, make a type and overload +=, ++, -=, --
// template on diff? and len?
// call it CycleIndex CircularIndex
size_t relative_index(const size_t pos, const size_t len, const int diff) {
  return (pos + len + diff) % len;
}

Outlet<char> make_intake() {

  auto incoming = make_pipe<char>();
  auto & [inlet, outlet] = incoming;

  const int source = relative_index(get_rank(), get_nprocs(), -1);
  const int dest = get_rank();

  inlet.EmplaceDuct<ProcessOutletDuct<char, 1024>>(
    MPI_COMM_WORLD,
    source,
    dest,
    0
  );

  return outlet;

}

Inlet<char> make_output() {

  auto incoming = make_pipe<char>();
  auto & [inlet, outlet] = incoming;

  const int source = get_rank();
  const int dest = relative_index(get_rank(), get_nprocs(), 1);

  inlet.EmplaceDuct<ProcessInletDuct<char, 1024>>(
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

  Outlet<char> intake = make_intake();

  const char message = 65 + get_rank();

  output.MaybePut(message);

  std::this_thread::sleep_for(std::chrono::seconds{1});

  char res;
  res = intake.GetCurrent();

  std::cout << "rank " << get_rank() << " sent " << message << std::endl;
  std::cout << "rank " << get_rank() << " received " << res << std::endl;

  MPI_Finalize();

  return 0;
}
