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
    df.AddVal(cfg.N_NODES_PER_CPU(), "Num Simulation Elements Per Cpu");
    df.AddVal(cfg.AMT_COMPUTE_WORK(), "Amount Compute Work");
    df.AddVal(uitsl::get_nprocs(), "Num Processes");
    df.AddVal(uitsl::get_exec_instance_uuid(), "Execution Instance UUID");
    df.AddVal(uitsl::get_proc_instance_uuid(), "Process Instance UUID");
    // SLURM_NNODES seems to be overwritten by mpiexec,
    // so allow for manual override
    df.AddVal(
      std::getenv("SLURM_NNODES_OVERRIDE") ?: std::getenv("SLURM_NNODES") ?: "",
      "SLURM_NNODES"
    );
    df.AddVal(std::getenv("SLURM_NTASKS") ?: "", "SLURM_NTASKS");
    df.AddVal(std::getenv("SLURM_CPUS_ON_NODE") ?: "", "SLURM_CPUS_ON_NODE");
    return df;
  }

  struct inlet {

    using inlet_t = inlet_instrumentation_aggregating_t;

    struct intra {

      static auto& GetContainerDataFile() {
        static auto res = AddBespokeColumns(
          inlet_t::intra::MakeContainerDataFile(
            emp::keyname::pack({
              {"a", "conduit_instrumentation"},
              {"async_mode", emp::to_string(cfg.ASYNCHRONOUS())},
              {"comp_work", emp::to_string(cfg.AMT_COMPUTE_WORK())},
              {"impl", "intra"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"subject", "inlet"},
              {
                "slurm_nnodes",
                std::getenv("SLURM_NNODES_OVERRIDE")
                  ?: std::getenv("SLURM_NNODES")
                  ?: ""
              },
              {"slurm_ntasks", std::getenv("SLURM_NTASKS") ?: ""},
              {"view", "container"},
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
          inlet_t::intra::MakeSummaryDataFile(
            emp::keyname::pack({
              {"a", "conduit_instrumentation"},
              {"async_mode", emp::to_string(cfg.ASYNCHRONOUS())},
              {"comp_work", emp::to_string(cfg.AMT_COMPUTE_WORK())},
              {"impl", "intra"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"subject", "inlet"},
              {
                "slurm_nnodes",
                std::getenv("SLURM_NNODES_OVERRIDE")
                  ?: std::getenv("SLURM_NNODES")
                  ?: ""
              },
              {"slurm_ntasks", std::getenv("SLURM_NTASKS") ?: ""},
              {"view", "summary"},
              {"_hostname", get_hostname()},
              {"_revision", EMP_STRINGIFY(REVISION_)},
              {"ext", ".csv"}
            })
          )
        );
        return res;
      }

    }; // struct intra

    struct thread {

      static auto& GetContainerDataFile() {
        static auto res = AddBespokeColumns(
          inlet_t::thread::MakeContainerDataFile(
            emp::keyname::pack({
              {"a", "conduit_instrumentation"},
              {"async_mode", emp::to_string(cfg.ASYNCHRONOUS())},
              {"comp_work", emp::to_string(cfg.AMT_COMPUTE_WORK())},
              {"impl", "thread"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"subject", "inlet"},
              {
                "slurm_nnodes",
                std::getenv("SLURM_NNODES_OVERRIDE")
                  ?: std::getenv("SLURM_NNODES")
                  ?: ""
              },
              {"slurm_ntasks", std::getenv("SLURM_NTASKS") ?: ""},
              {"view", "container"},
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
              {"comp_work", emp::to_string(cfg.AMT_COMPUTE_WORK())},
              {"impl", "thread"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"subject", "inlet"},
              {
                "slurm_nnodes",
                std::getenv("SLURM_NNODES_OVERRIDE")
                  ?: std::getenv("SLURM_NNODES")
                  ?: ""
              },
              {"slurm_ntasks", std::getenv("SLURM_NTASKS") ?: ""},
              {"view", "summary"},
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
              {"comp_work", emp::to_string(cfg.AMT_COMPUTE_WORK())},
              {"impl", "proc"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"subject", "inlet"},
              {
                "slurm_nnodes",
                std::getenv("SLURM_NNODES_OVERRIDE")
                  ?: std::getenv("SLURM_NNODES")
                  ?: ""
              },
              {"slurm_ntasks", std::getenv("SLURM_NTASKS") ?: ""},
              {"view", "container"},
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
              {"comp_work", emp::to_string(cfg.AMT_COMPUTE_WORK())},
              {"impl", "proc"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"subject", "inlet"},
              {
                "slurm_nnodes",
                std::getenv("SLURM_NNODES_OVERRIDE")
                  ?: std::getenv("SLURM_NNODES")
                  ?: ""
              },
              {"slurm_ntasks", std::getenv("SLURM_NTASKS") ?: ""},
              {"view", "summary"},
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

    struct intra {

      static auto& GetContainerDataFile() {
        static auto res = AddBespokeColumns(
          outlet_t::intra::MakeContainerDataFile(
            emp::keyname::pack({
              {"a", "conduit_instrumentation"},
              {"async_mode", emp::to_string(cfg.ASYNCHRONOUS())},
              {"comp_work", emp::to_string(cfg.AMT_COMPUTE_WORK())},
              {"impl", "intra"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"subject", "outlet"},
              {
                "slurm_nnodes",
                std::getenv("SLURM_NNODES_OVERRIDE")
                  ?: std::getenv("SLURM_NNODES")
                  ?: ""
              },
              {"slurm_ntasks", std::getenv("SLURM_NTASKS") ?: ""},
              {"view", "container"},
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
          outlet_t::intra::MakeSummaryDataFile(
            emp::keyname::pack({
              {"a", "conduit_instrumentation"},
              {"async_mode", emp::to_string(cfg.ASYNCHRONOUS())},
              {"comp_work", emp::to_string(cfg.AMT_COMPUTE_WORK())},
              {"impl", "intra"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"subject", "outlet"},
              {
                "slurm_nnodes",
                std::getenv("SLURM_NNODES_OVERRIDE")
                  ?: std::getenv("SLURM_NNODES")
                  ?: ""
              },
              {"slurm_ntasks", std::getenv("SLURM_NTASKS") ?: ""},
              {"view", "summary"},
              {"_hostname", get_hostname()},
              {"_revision", EMP_STRINGIFY(REVISION_)},
              {"ext", ".csv"}
            })
          )
        );
        return res;
      }

    }; // struct intra

    struct thread {

      static auto& GetContainerDataFile() {
        static auto res = AddBespokeColumns(
          outlet_t::thread::MakeContainerDataFile(
            emp::keyname::pack({
              {"a", "conduit_instrumentation"},
              {"async_mode", emp::to_string(cfg.ASYNCHRONOUS())},
              {"comp_work", emp::to_string(cfg.AMT_COMPUTE_WORK())},
              {"impl", "thread"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"subject", "outlet"},
              {
                "slurm_nnodes",
                std::getenv("SLURM_NNODES_OVERRIDE")
                  ?: std::getenv("SLURM_NNODES")
                  ?: ""
              },
              {"slurm_ntasks", std::getenv("SLURM_NTASKS") ?: ""},
              {"view", "container"},
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
              {"comp_work", emp::to_string(cfg.AMT_COMPUTE_WORK())},
              {"impl", "thread"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"subject", "outlet"},
              {
                "slurm_nnodes",
                std::getenv("SLURM_NNODES_OVERRIDE")
                  ?: std::getenv("SLURM_NNODES")
                  ?: ""
              },
              {"slurm_ntasks", std::getenv("SLURM_NTASKS") ?: ""},
              {"view", "summary"},
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
              {"comp_work", emp::to_string(cfg.AMT_COMPUTE_WORK())},
              {"impl", "proc"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"subject", "outlet"},
              {
                "slurm_nnodes",
                std::getenv("SLURM_NNODES_OVERRIDE")
                  ?: std::getenv("SLURM_NNODES")
                  ?: ""
              },
              {"slurm_ntasks", std::getenv("SLURM_NTASKS") ?: ""},
              {"view", "container"},
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
              {"comp_work", emp::to_string(cfg.AMT_COMPUTE_WORK())},
              {"impl", "proc"},
              {"nproc", emp::to_string(uitsl::get_nprocs())},
              {"nthread", emp::to_string(cfg.N_THREADS())},
              {"proc", emp::to_string(uitsl::get_proc_id())},
              {"replicate", emp::to_string(cfg.REPLICATE())},
              {"subject", "outlet"},
              {
                "slurm_nnodes",
                std::getenv("SLURM_NNODES_OVERRIDE")
                  ?: std::getenv("SLURM_NNODES")
                  ?: ""
              },
              {"slurm_ntasks", std::getenv("SLURM_NTASKS") ?: ""},
              {"view", "summary"},
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

    if ( cfg.N_THREADS() == 1 && !uitsl::is_multiprocess() ) {
      if ( cfg.WRITE_CONTAINER_INSTRUMENTATION_DATAFILES() ) {
        inlet::intra::GetContainerDataFile().PrintHeaderKeys();
        outlet::intra::GetContainerDataFile().PrintHeaderKeys();
      }

      if ( cfg.WRITE_SUMMARY_INSTRUMENTATION_DATAFILES() ) {
        inlet::intra::GetSummaryDataFile().PrintHeaderKeys();
        outlet::intra::GetSummaryDataFile().PrintHeaderKeys();
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

    if ( cfg.N_THREADS() == 1 && !uitsl::is_multiprocess() ) {
      if ( cfg.WRITE_CONTAINER_INSTRUMENTATION_DATAFILES() ) {
        inlet::intra::GetContainerDataFile().Update();
        outlet::intra::GetContainerDataFile().Update();
      }

      if ( cfg.WRITE_SUMMARY_INSTRUMENTATION_DATAFILES() ) {
        inlet::intra::GetSummaryDataFile().Update();
        outlet::intra::GetSummaryDataFile().Update();
      }
    }

    ++update;
  }

};
