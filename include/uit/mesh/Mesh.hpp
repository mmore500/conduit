#pragma once

#include <unordered_map>
#include <stddef.h>

#include <mpi.h>

#include "../conduit/Duct.hpp"
#include "../mpi/mpi_utils.hpp"
#include "../utility/assign_utils.hpp"
#include "../topology/Topology.hpp"

#include "MeshNode.hpp"
#include "MeshTopology.hpp"

namespace uit {

namespace internal {

class MeshIDCounter {

  static inline size_t counter{};

public:

  static size_t Generate() { return counter++; }

};

} // namespace internal

template<typename ImplSpec>
class Mesh {

  using node_id_t = size_t;
  using edge_id_t = size_t;
  using node_t = MeshNode<ImplSpec>;

  inline static size_t mesh_id_counter{};
  size_t mesh_id;
  MPI_Comm comm;

  // node_id -> node
  internal::MeshTopology<ImplSpec> nodes;

  std::function<uit::thread_id_t(node_id_t)> thread_assignment;
  std::function<uit::proc_id_t(node_id_t)> proc_assignment;

  using back_end_t = typename ImplSpec::ProcBackEnd;
  std::shared_ptr<back_end_t> back_end{ std::make_shared<back_end_t>() };

  void InitializeInterThreadDucts() {
    for (auto& [node_id, node] : nodes) {
      InitializeInterThreadDucts(node_id, node);
    }
  }

  void InitializeInterThreadDucts(const node_id_t node_id, node_t & node) {
    // only need to iterate through inputs because this fixes outputs' ducts too
    for (auto& input : node.GetInputs()) InitializeInterThreadDuct(input);
  }

  void InitializeInterThreadDuct(uit::MeshNodeInput<ImplSpec> & input) {

    const node_id_t inlet_node_id = nodes.GetOutputRegistry().at(
      input.GetEdgeID()
    );
    const uit::thread_id_t inlet_thread = thread_assignment(inlet_node_id);

    const node_id_t outlet_node_id = nodes.GetInputRegistry().at(
      input.GetEdgeID()
    );
    const uit::thread_id_t outlet_thread = thread_assignment(outlet_node_id);

    if (inlet_thread != outlet_thread) input.template EmplaceDuct<
      typename ImplSpec::ThreadDuct
    >();

  }

  void InitializeInterProcDucts() {
    for (auto& [node_id, node] : nodes) {
      InitializeInterProcDucts(node_id, node);
    }
  }

  void InitializeInterProcDucts(const node_id_t node_id, node_t& node) {

    for (auto & input : node.GetInputs()) InitializeInterProcDuct(input);

    for (auto & output : node.GetOutputs()) InitializeInterProcDuct(output);

  }

  void InitializeInterProcDuct(uit::MeshNodeInput<ImplSpec>& input) {
    const node_id_t inlet_node_id = nodes.GetOutputRegistry().at(
      input.GetEdgeID()
    );
    const uit::proc_id_t inlet_proc_id = proc_assignment(inlet_node_id);

    const node_id_t outlet_node_id = nodes.GetInputRegistry().at(
      input.GetEdgeID()
    );
    const uit::proc_id_t outlet_proc_id = proc_assignment(outlet_node_id);

    static std::unordered_set<int> tag_checker;
    const int tag = uit::combine_tag(mesh_id, input.GetEdgeID());

    // assert that generated tags are unique
    emp_assert( tag_checker.insert(tag).second );

    const uit::InterProcAddress addr{
      outlet_proc_id,
      inlet_proc_id,
      thread_assignment(outlet_node_id),
      thread_assignment(inlet_node_id),
      tag,
      comm
    };

    if (inlet_proc_id != outlet_proc_id) input.template SplitDuct<
      typename ImplSpec::ProcOutletDuct
    >(addr, back_end);

  }

  void InitializeInterProcDuct(uit::MeshNodeOutput<ImplSpec>& output) {
    const node_id_t inlet_node_id = nodes.GetOutputRegistry().at(
      output.GetEdgeID()
    );
    const uit::proc_id_t inlet_proc_id = proc_assignment(inlet_node_id);

    const node_id_t outlet_node_id = nodes.GetInputRegistry().at(
      output.GetEdgeID()
    );
    const uit::proc_id_t outlet_proc_id = proc_assignment(outlet_node_id);

    const uit::InterProcAddress addr{
      outlet_proc_id,
      inlet_proc_id,
      thread_assignment(outlet_node_id),
      thread_assignment(inlet_node_id),
      uit::combine_tag(mesh_id, output.GetEdgeID()),
      comm
    };

    if (inlet_proc_id != outlet_proc_id) output.template SplitDuct<
      typename ImplSpec::ProcInletDuct
    >(addr, back_end);


  }


public:

  Mesh(
    const Topology & topology,
    const std::function<thread_id_t(node_id_t)> thread_assignment_
      =uit::AssignIntegrated<thread_id_t>{},
    const std::function<proc_id_t(node_id_t)> proc_assignment_
      =uit::AssignIntegrated<proc_id_t>{},
    const MPI_Comm comm_=MPI_COMM_WORLD,
    const size_t mesh_id_=internal::MeshIDCounter::Generate()
  )
  : mesh_id(mesh_id_)
  , comm(comm_)
  , nodes(topology, proc_assignment_, comm)
  , thread_assignment(thread_assignment_)
  , proc_assignment(proc_assignment_) {
    InitializeInterThreadDucts();
    InitializeInterProcDucts();
    back_end->Initialize();
  }

  // TODO rename GetNumNodes
  size_t GetNodeCount() const { return nodes.GetNodeCount(); }

  // TODO rename GetNumEdges
  size_t GetEdgeCount() const { return nodes.GetEdgeCount(); }

  using submesh_t = emp::vector<node_t>;

  submesh_t GetSubmesh(const thread_id_t tid=0) const {
    return GetSubmesh(tid, uit::get_proc_id(comm));
  }

  submesh_t GetSubmesh(
    const thread_id_t tid,
    const proc_id_t pid
  ) const {
    submesh_t res;
    for (const auto& [node_id, node] : nodes) {
      if (
        thread_assignment(node_id) == tid
        && proc_assignment(node_id) == pid
      ) res.push_back(node);
    }
    return res;
  }

  std::string ToString() const {
    std::stringstream ss;
    ss << "TODO" << std::endl;
    return ss.str();
  }

};

} // namespace uit
