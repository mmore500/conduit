#include <chrono>
#include <iostream>
#include <map>
#include <sstream>

#include "uit/distributed/assign_utils.hpp"
#include "uit/distributed/do_successively.hpp"
#include "uit/containers/safe/unordered_map.hpp"
#include "uit/mpi/MpiMultithreadGuard.hpp"
#include "uit/parallel/ThreadTeam.hpp"
#include "uit/utility/assign_utils.hpp"
#include "uit/utility/numeric_cast.hpp"
#include "uit/topology/RingTopologyFactory.hpp"

#include "Job.hpp"

const uit::MpiMultithreadGuard guard{};

constexpr size_t num_threads = 2;
const size_t num_procs = uit::numeric_cast<size_t>( uit::get_nprocs() );

constexpr size_t nodes_per_job = 1;
const size_t num_nodes = num_procs * num_threads * nodes_per_job;

int main() {

  if ( uit::is_root() ) std::cout << ">>> begin <<<" << std::endl << std::endl;

  uit::Mesh<ImplSpec> mesh{
    uit::RingTopologyFactory{}( num_nodes ),
    uit::AssignRoundRobin<uit::thread_id_t>{ num_threads, nodes_per_job },
    uit::AssignContiguously<uit::proc_id_t>{ num_procs, num_nodes }
  };

  uit::safe::unordered_map<size_t, std::string> res;

  uit::ThreadTeam team;
  for (size_t thread = 0; thread < num_threads; ++thread) team.Add(
    [&mesh, &res, thread](){
      Job job{ mesh.GetSubmesh(thread) };

      std::stringstream ss;

      ss << "process " << uit::get_proc_id() << std::endl;
      ss << "thread " << thread << std::endl;
      ss << job.ToString() << std::endl;

      res[thread] = ss.str();

    }
  );
  team.Join();

  uit::do_successively(
    [&](){
      for (const auto& [k, v] : std::map{ std::begin(res), std::end(res) }) {
        std::cout << v;
      }
    },
    uit::print_separator
  );


  if ( uit::is_root() ) std::cout << ">>> end <<<" << std::endl;

  return 0;
}
