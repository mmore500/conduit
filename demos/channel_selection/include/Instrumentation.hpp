#pragma once

#include <cstdlib>
#include <variant>

#include "../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "uitsl/mpi/comm_utils.hpp"
#include "uitsl/utility/uuid_utils.hpp"

#include "config/cfg.hpp"
#include "get_hostname.hpp"
#include "typedef.hpp"

class Instrumentation {

  inline static size_t update{};
  inline static size_t snapshot{};
  inline static bool has_execution_blur;

  template<typename DATAFILE>
  static auto AddBespokeColumns(DATAFILE df) {
    df.AddVar(update, "Update");
    df.AddVar(snapshot, "Snapshot");
    df.AddVar(has_execution_blur, "Has Execution Blur");
    df.AddVal(cfg.REPLICATE(), "Replicate");
    df.AddVal(cfg.ASYNCHRONOUS(), "Async Mode");
    df.AddVal(cfg.N_THREADS(), "Num Threads");
    df.AddVal(uitsl::get_nprocs(), "Num Processes");
    df.AddVal(uitsl::get_exec_instance_uuid(), "Execution Instance UUID");
    df.AddVal(uitsl::get_proc_instance_uuid(), "Process Instance UUID");
    df.AddVal(std::getenv("SLURM_NNODES") ?: "", "SLURM_NNODES");
    df.AddVal(std::getenv("SLURM_NTASKS") ?: "", "SLURM_NTASKS");
    df.AddVal(std::getenv("SLURM_CPUS_ON_NODE") ?: "", "SLURM_CPUS_ON_NODE");
    return df;
  }

  struct inlet {

    using inlet_t = inlet_instrumentation_aggregating_t;

    struct thread {

      static auto& GetContainerDataFile() {
        static auto res = AddBespokeColumns(
          inlet_t::thread::MakeContainerDataFile(
            emp::keyname::pack({
              {"a", "conduit_instrumentation"},
              {"async_mode", emp::to_string(cfg.ASYNCHRONOUS())},
              {"impl", "thread"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"subject", "inlet"},
              {"view", "container"},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"_hostname", get_hostname()},
              {"_revision", EMP_STRINGIFY(REVISION_)},
              {"ext", ".csv"}
            })
          )
        );
        return res;
      }

      static auto& GetSummaryDataFile() {
        static auto res = AddBespokeColumns(
          inlet_t::thread::MakeSummaryDataFile(
            emp::keyname::pack({
              {"a", "conduit_instrumentation"},
              {"async_mode", emp::to_string(cfg.ASYNCHRONOUS())},
              {"impl", "thread"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"subject", "inlet"},
              {"view", "summary"},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"_hostname", get_hostname()},
              {"_revision", EMP_STRINGIFY(REVISION_)},
              {"ext", ".csv"}
            })
          )
        );
        return res;
      }

    }; // struct thread

    struct proc {

      static auto& GetContainerDataFile() {
        static auto res = AddBespokeColumns(
          inlet_t::proc::MakeContainerDataFile(
            emp::keyname::pack({
              {"a", "conduit_instrumentation"},
              {"async_mode", emp::to_string(cfg.ASYNCHRONOUS())},
              {"impl", "proc"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"subject", "inlet"},
              {"view", "container"},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"_hostname", get_hostname()},
              {"_revision", EMP_STRINGIFY(REVISION_)},
              {"ext", ".csv"}
            })
          )
        );
        return res;
      }

      static auto& GetSummaryDataFile() {
        static auto res = AddBespokeColumns(
          inlet_t::proc::MakeSummaryDataFile(
            emp::keyname::pack({
              {"a", "conduit_instrumentation"},
              {"async_mode", emp::to_string(cfg.ASYNCHRONOUS())},
              {"impl", "proc"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"subject", "inlet"},
              {"view", "summary"},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"_hostname", get_hostname()},
              {"_revision", EMP_STRINGIFY(REVISION_)},
              {"ext", ".csv"}
            })
          )
        );
        return res;
      }

    }; // struct proc

  }; // struct inlet

  struct outlet {

    using outlet_t = outlet_instrumentation_aggregating_t;

    struct thread {

      static auto& GetContainerDataFile() {
        static auto res = AddBespokeColumns(
          outlet_t::thread::MakeContainerDataFile(
            emp::keyname::pack({
              {"a", "conduit_instrumentation"},
              {"async_mode", emp::to_string(cfg.ASYNCHRONOUS())},
              {"impl", "thread"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"subject", "outlet"},
              {"view", "container"},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"_hostname", get_hostname()},
              {"_revision", EMP_STRINGIFY(REVISION_)},
              {"ext", ".csv"}
            })
          )
        );
        return res;
      }

      static auto& GetSummaryDataFile() {
        static auto res = AddBespokeColumns(
          outlet_t::thread::MakeSummaryDataFile(
            emp::keyname::pack({
              {"a", "conduit_instrumentation"},
              {"async_mode", emp::to_string(cfg.ASYNCHRONOUS())},
              {"impl", "thread"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"subject", "outlet"},
              {"view", "summary"},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"_hostname", get_hostname()},
              {"_revision", EMP_STRINGIFY(REVISION_)},
              {"ext", ".csv"}
            })
          )
        );
        return res;
      }

    }; // struct thread

    struct proc {

      static auto& GetContainerDataFile() {
        static auto res = AddBespokeColumns(
          outlet_t::proc::MakeContainerDataFile(
            emp::keyname::pack({
              {"a", "conduit_instrumentation"},
              {"async_mode", emp::to_string(cfg.ASYNCHRONOUS())},
              {"impl", "proc"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"subject", "outlet"},
              {"view", "container"},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"_hostname", get_hostname()},
              {"_revision", EMP_STRINGIFY(REVISION_)},
              {"ext", ".csv"}
            })
          )
        );
        return res;
      }

      static auto& GetSummaryDataFile() {
        static auto res = AddBespokeColumns(
          outlet_t::proc::MakeSummaryDataFile(
            emp::keyname::pack({
              {"a", "conduit_instrumentation"},
              {"async_mode", emp::to_string(cfg.ASYNCHRONOUS())},
              {"impl", "proc"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"subject", "outlet"},
              {"view", "summary"},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"_hostname", get_hostname()},
              {"_revision", EMP_STRINGIFY(REVISION_)},
              {"ext", ".csv"}
            })
          )
        );
        return res;
      }

    }; // struct proc

  }; // struct outlet

public:

  static void ElapseShapshot() { ++snapshot; }

  static void PrintHeaderKeys() {
    if ( cfg.N_THREADS() > 1 ) {
      if ( cfg.WRITE_CONTAINER_INSTRUMENTATION_DATAFILES() ) {
        inlet::thread::GetContainerDataFile().PrintHeaderKeys();
        outlet::thread::GetContainerDataFile().PrintHeaderKeys();
      }

      if ( cfg.WRITE_SUMMARY_INSTRUMENTATION_DATAFILES() ) {
        inlet::thread::GetSummaryDataFile().PrintHeaderKeys();
        outlet::thread::GetSummaryDataFile().PrintHeaderKeys();
      }
    }

    if ( uitsl::is_multiprocess() ) {
      if ( cfg.WRITE_CONTAINER_INSTRUMENTATION_DATAFILES() ) {
        inlet::proc::GetContainerDataFile().PrintHeaderKeys();
        outlet::proc::GetContainerDataFile().PrintHeaderKeys();
      }

      if ( cfg.WRITE_SUMMARY_INSTRUMENTATION_DATAFILES() ) {
        inlet::proc::GetSummaryDataFile().PrintHeaderKeys();
        outlet::proc::GetSummaryDataFile().PrintHeaderKeys();
      }
    }
  }

  static void UpdateDataFiles(const bool has_execution_blur_) {

    has_execution_blur = has_execution_blur_;

    if ( cfg.N_THREADS() > 1 ) {
      if ( cfg.WRITE_CONTAINER_INSTRUMENTATION_DATAFILES() ) {
        inlet::thread::GetContainerDataFile().Update();
        outlet::thread::GetContainerDataFile().Update();
      }

      if ( cfg.WRITE_SUMMARY_INSTRUMENTATION_DATAFILES() ) {
        inlet::thread::GetSummaryDataFile().Update();
        outlet::thread::GetSummaryDataFile().Update();
      }
    }

    if ( uitsl::is_multiprocess() ) {
      if ( cfg.WRITE_CONTAINER_INSTRUMENTATION_DATAFILES() ) {
        inlet::proc::GetContainerDataFile().Update();
        outlet::proc::GetContainerDataFile().Update();
      }

      if ( cfg.WRITE_SUMMARY_INSTRUMENTATION_DATAFILES() ) {
        inlet::proc::GetSummaryDataFile().Update();
        outlet::proc::GetSummaryDataFile().Update();
      }
    }
    ++update;
  }

};
