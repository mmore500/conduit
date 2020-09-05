#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/distributed/DistributedTimeoutBarrier.hpp"
#include "uit/mpi/MpiGuard.hpp"
#include "uit/debug/MultiprocessReporter.hpp"

const uit::MpiGuard guard;

TEST_CASE("test do_successively") {

  uit::do_successively(
    [=](){ std::cout << "hello" << std::endl; },
    uit::print_separator
  );

}
