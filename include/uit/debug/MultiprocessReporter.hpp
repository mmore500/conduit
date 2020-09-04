#pragma once

#include <mpi.h>

#include "../../../third-party/Catch/single_include/catch2/catch.hpp"

#include "../mpi/mpi_utils.hpp"

namespace uit {

class MultiprocessReporter : public Catch::ConsoleReporter {

  Catch::ConsoleReporter impl;

public:

  MultiprocessReporter(const Catch::ReporterConfig& config)
  : Catch::ConsoleReporter(config), impl(config) { ; }

  void testRunEnded(Catch::TestRunStats const& testRunStats) override {

    if(uit::is_root() || !testRunStats.totals.testCases.allPassed()) {
      const std::string message{ emp::to_string(
        "\x1B[35m",
        "Processes: ",
        uit::get_nprocs(),
        "\033[0m\n"
      ) };
      printf("%s", message.c_str());
      impl.testRunEnded(testRunStats);
    }

  }

};

CATCH_REGISTER_REPORTER ("multiprocess", MultiprocessReporter)

} // namespace uit
