#include <chrono>
#include <iostream>
#include <map>
#include <sstream>

#include "../../../third-party/Empirical/include/emp/config/ArgManager.hpp"

#include "netuit/arrange/RingTopologyFactory.hpp"
#include "netuit/assign/AssignAvailableProcs.hpp"
#include "uitsl/containers/safe/unordered_map.hpp"
#include "uitsl/debug/safe_cast.hpp"
#include "uitsl/distributed/do_successively.hpp"
#include "uitsl/mpi/comm_utils.hpp"
#include "uitsl/mpi/mpi_flex_guard.hpp"
#include "uitsl/parallel/ThreadTeam.hpp"
#include "uitsl/utility/assign_utils.hpp"

#include "config/num_nodes.hpp"

#include "Job.hpp"

int main(int argc, char* argv[]) {

  emp::ArgManager am{ argc, argv, emp::ArgManager::make_builtin_specs(&cfg) };
  am.UseCallbacks();
  if ( am.HasUnused() ) std::exit( EXIT_FAILURE );

  if ( cfg.N_THREADS() == 1 ) uitsl::mpi_flex_guard.InitSingleThread();
  else uitsl::mpi_flex_guard.InitMultithread();

  if ( uitsl::is_root() ) {
    std::cout << ">>> begin <<<" << std::endl << std::endl;
  }

  // todo switch this out for assign metis
  netuit::Mesh<ImplSpec> mesh{
    netuit::RingTopologyFactory{}( num_nodes() ),
    uitsl::AssignRoundRobin<uitsl::thread_id_t>{
      cfg.N_THREADS(), cfg.N_NODES_PER_CPU()
    },
    uitsl::AssignContiguously<uitsl::proc_id_t>{
      uitsl::safe_cast<size_t>(uitsl::get_nprocs()), num_nodes()
    }
  };

  uitsl::safe::unordered_map<size_t, std::string> res;

  uitsl::ThreadTeam team;
  for (size_t thread = 0; thread < cfg.N_THREADS(); ++thread) team.Add(
    [&mesh, &res, thread](){

      // run the job
      Job job{ thread, mesh.GetSubmesh(thread) };

      std::stringstream ss;

      ss << "process " << uitsl::get_proc_id() << std::endl;
      ss << "thread " << thread << std::endl;
      ss << job.ToString() << std::endl;

      res[thread] = ss.str();

    }
  );
  team.Join();

  uitsl::do_successively(
    [&](){
      for (const auto& [k, v] : std::map{ std::begin(res), std::end(res) }) {
        std::cout << v;
      }
    },
    uitsl::print_separator
  );

  return 0;
}
