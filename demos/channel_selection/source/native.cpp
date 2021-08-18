#include <chrono>
#include <iostream>
#include <map>
#include <sstream>

#include "../../../third-party/Empirical/include/emp/config/ArgManager.hpp"

#include "netuit/arrange/ToroidalTopologyFactory.hpp"
#include "netuit/assign/GenerateMetisAssignments.hpp"
#include "uitsl/containers/safe/unordered_map.hpp"
#include "uitsl/countdown/FixedRateRepeatingTimer.hpp"
#include "uitsl/countdown/Timer.hpp"
#include "uitsl/debug/safe_cast.hpp"
#include "uitsl/distributed/do_successively.hpp"
#include "uitsl/mpi/comm_utils.hpp"
#include "uitsl/mpi/mpi_flex_guard.hpp"
#include "uitsl/parallel/ThreadTeam.hpp"
#include "uitsl/polyfill/latch.hpp"
#include "uitsl/utility/assign_utils.hpp"

#include "config/num_nodes.hpp"

#include "Instrumentation.hpp"
#include "Job.hpp"

int main(int argc, char* argv[]) {

  emp::ArgManager am{ argc, argv, emp::ArgManager::make_builtin_specs(&cfg) };
  am.UseCallbacks();
  if ( am.HasUnused() ) std::exit( EXIT_FAILURE );

  if ( cfg.N_THREADS() == 1 ) uitsl::mpi_flex_guard.InitSingleThread();
  else uitsl::mpi_flex_guard.InitMultithread();

  if ( uitsl::is_root() ) {
    std::cout << ">>> begin <<<" << '\n' << '\n';
  }

  const size_t dim = static_cast<size_t>(std::pow(num_nodes(), 0.5));
  const auto topology = netuit::make_toroidal_topology(
    { dim, dim }
  );

  const std::pair<
      std::function<uitsl::proc_id_t(size_t)>,
      std::function<uitsl::thread_id_t(size_t)>
  > assignments = netuit::GenerateMetisAssignmentFunctors(
    uitsl::safe_cast<size_t>( uitsl::get_nprocs() ),
    cfg.N_THREADS(),
    topology
  );

  netuit::Mesh<ImplSpec> mesh{
    topology,
    assignments.second,
    assignments.first
  };

  uitsl::safe::unordered_map<size_t, std::string> res;

  std::latch mesh_disposal_latch{
    static_cast<int>( cfg.N_THREADS() )
  };

  uitsl::ThreadTeam team;
  for (size_t thread = 0; thread < cfg.N_THREADS(); ++thread) team.Add(
    [&mesh, &res,&mesh_disposal_latch,  thread](){

      // set up the job
      Job job{ mesh.GetSubmesh(thread) };

      mesh_disposal_latch.count_down();

      // run the job
      job.Run(thread);

      std::stringstream ss;

      ss << "process " << uitsl::get_proc_id() << '\n';
      ss << "thread " << thread << '\n';
      ss << job.ToString() << '\n';

      res[thread] = ss.str();

    }
  );

  mesh_disposal_latch.wait();
  mesh.~Mesh();

  Instrumentation::PrintHeaderKeys();

  uitsl::FixedRateRepeatingTimer snapshot_interval_timer{
    cfg.SNAPSHOT_INTERVAL()
  };
  while ( !team.TryJoin() ) {

    while ( !snapshot_interval_timer.IsComplete() && !team.TryJoin() );
    if ( team.TryJoin() ) break;

    Instrumentation::UpdateDataFiles();

    const uitsl::Timer snapshot_duration_timer{ cfg.SNAPSHOT_DURATION() };
    while ( !snapshot_duration_timer.IsComplete() );

    Instrumentation::UpdateDataFiles();
    Instrumentation::ElapseShapshot();
  }

  uitsl::do_successively(
    [&](){
      for (const auto& [k, v] : std::map{ std::begin(res), std::end(res) }) {
        // prevent excessive logs for many-proc jobs
        if (uitsl::get_proc_id() < 4) std::cout << v;
      }
    },
    uitsl::print_separator
  );

  return 0;
}
