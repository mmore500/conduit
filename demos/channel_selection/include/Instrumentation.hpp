#pragma once

#include <variant>

#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "uitsl/mpi/comm_utils.hpp"

#include "config/cfg.hpp"
#include "typedef.hpp"

class Instrumentation {

  struct inlet {

    using inlet_t = inlet_instrumentation_aggregating_t;

    struct thread {

      static auto& GetContainerDataFile() {
        static auto res = inlet_t::thread::MakeContainerDataFile(
          emp::keyname::pack({
            {"a", "conduit_instrumentation"},
            {"impl", "thread"},
            {"subject", "inlet"},
            {"view", "container"}
          })
        );
        return res;
      }

      static auto& GetSummaryDataFile() {
        static auto res = inlet_t::thread::MakeSummaryDataFile(
          emp::keyname::pack({
            {"a", "conduit_instrumentation"},
            {"impl", "thread"},
            {"subject", "inlet"},
            {"view", "summary"}
          })
        );
        return res;
      }

    }; // struct thread

    struct proc {

      static auto& GetContainerDataFile() {
        static auto res = inlet_t::proc::MakeContainerDataFile(
          emp::keyname::pack({
            {"a", "conduit_instrumentation"},
            {"impl", "proc"},
            {"subject", "inlet"},
            {"view", "container"}
          })
        );
        return res;
      }

      static auto& GetSummaryDataFile() {
        static auto res = inlet_t::proc::MakeSummaryDataFile(
          emp::keyname::pack({
            {"a", "conduit_instrumentation"},
            {"impl", "proc"},
            {"subject", "inlet"},
            {"view", "summary"}
          })
        );
        return res;
      }

    }; // struct proc

  }; // struct inlet

  struct outlet {

    using outlet_t = outlet_instrumentation_aggregating_t;

    struct thread {

      static auto& GetContainerDataFile() {
        static auto res = outlet_t::thread::MakeContainerDataFile(
          emp::keyname::pack({
            {"impl", "thread"},
            {"subject", "outlet"},
            {"view", "container"}
          })
        );
        return res;
      }

      static auto& GetSummaryDataFile() {
        static auto res = outlet_t::thread::MakeSummaryDataFile(
          emp::keyname::pack({
            {"impl", "thread"},
            {"subject", "outlet"},
            {"view", "summary"}
          })
        );
        return res;
      }

    }; // struct thread

    struct proc {

      static auto& GetContainerDataFile() {
        static auto res = outlet_t::proc::MakeContainerDataFile(
          emp::keyname::pack({
            {"impl", "proc"},
            {"subject", "outlet"},
            {"view", "container"}
          })
        );
        return res;
      }

      static auto& GetSummaryDataFile() {
        static auto res = outlet_t::proc::MakeSummaryDataFile(
          emp::keyname::pack({
            {"impl", "proc"},
            {"subject", "outlet"},
            {"view", "summary"}
          })
        );
        return res;
      }

    }; // struct proc

  }; // struct outlet

public:

  static void PrintHeaderKeys() {
    if ( cfg.N_THREADS() > 1 ) {
      inlet::thread::GetContainerDataFile().PrintHeaderKeys();
      inlet::thread::GetSummaryDataFile().PrintHeaderKeys();
      outlet::thread::GetContainerDataFile().PrintHeaderKeys();
      outlet::thread::GetSummaryDataFile().PrintHeaderKeys();
    }

    if ( uitsl::is_multiprocess() ) {
      inlet::proc::GetContainerDataFile().PrintHeaderKeys();
      inlet::proc::GetSummaryDataFile().PrintHeaderKeys();
      outlet::proc::GetContainerDataFile().PrintHeaderKeys();
      outlet::proc::GetSummaryDataFile().PrintHeaderKeys();
    }
  }

  static void UpdateDataFiles() {
    if ( cfg.N_THREADS() > 1 ) {
      inlet::thread::GetContainerDataFile().Update();
      inlet::thread::GetSummaryDataFile().Update();
      outlet::thread::GetContainerDataFile().Update();
      outlet::thread::GetSummaryDataFile().Update();
    }

    if ( uitsl::is_multiprocess() ) {
      inlet::proc::GetContainerDataFile().Update();
      inlet::proc::GetSummaryDataFile().Update();
      outlet::proc::GetContainerDataFile().Update();
      outlet::proc::GetSummaryDataFile().Update();
    }

  }

};
