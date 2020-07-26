#pragma once

#include <unordered_map>

#include "Duct.h"

#include "mesh_utils.h"

template<typename T, size_t N=DEFAULT_BUFFER>
class Mesh {

  using node_id_t = size_t;
  using pipe_id_t = size_t;

  mesh_t<T, N> mesh;

  // pipe_id -> node_id
  std::unordered_map<size_t, size_t> input_registry;
  std::unordered_map<size_t, size_t> output_registry;

  void InitializeRegistries() {
    for (size_t node_id = 0; node_id < mesh.size(); ++node_id) {
      auto & node = mesh[node_id];
      auto & [inputs, outputs] = node;

      for (auto & link : inputs) {
        emp_assert(input_registry.count(link) == 0);
        input_registry[link.GetPipeID()] = node_id;
      }

      for (auto & link : outputs) {
        emp_assert(output_registry.count(link) == 0);
        output_registry[link.GetPipeID()] = node_id;
      }

    }
  }

  void InitializeInterThreadPipes(
    const std::function<thread_id_t(node_id_t)> thread_assignment
  ) {

    for (size_t node_id = 0; node_id < mesh.size(); ++node_id) {
      auto & node = mesh[node_id];
      auto & [inputs, outputs] = node;
      const thread_id_t my_thread = thread_assignment(node_id);

      for (auto & link : inputs) {
        const node_id_t their_node = output_registry[link];
        const thread_id_t their_thread = thread_assignment(their_node);

        if (my_thread != their_thread) {
          link.GetInput().template EmplaceDuct<
            ThreadDuct<T, N>
          >();
        }
      }

    }

  }

  void InitializeInterProcPipes(
    const std::function<proc_id_t(node_id_t)> proc_assignment
  ) {

    for (size_t node_id = 0; node_id < mesh.size(); ++node_id) {
      auto & node = mesh[node_id];
      auto & [inputs, outputs] = node;
      const proc_id_t my_proc = proc_assignment(node_id);

      for (auto & link : inputs) {
        const node_id_t their_node = output_registry[link];
        const proc_id_t their_proc = proc_assignment(their_node);

        if (my_proc != their_proc) {
          link.GetInput().template SplitDuct<
            ProcessInletDuct<T, N>
          >(
            their_proc,
            link.GetPipeID()
          );
        }
      }

      for (auto & link : outputs) {
        const node_id_t their_node = input_registry[link];
        const proc_id_t their_proc = proc_assignment(their_node);

        if (my_proc != their_proc) {
          link.GetOutput().template SplitDuct<
            ProcessOutletDuct<T, N>
          >(
            their_proc,
            link.GetPipeID()
          );
        }
      }

    }

  }

public:

  using value_type = io_bundle_t<T, N>;

  Mesh(
    const mesh_t<T, N> & mesh_,
    const std::function<thread_id_t(node_id_t)> thread_assignment,
    const std::function<proc_id_t(node_id_t)> proc_assignment
      =[](const auto & node_id){ return 0; }
  )
  : mesh(mesh_) {
    InitializeRegistries();
    InitializeInterThreadPipes(thread_assignment);
    InitializeInterProcPipes(proc_assignment);
  }

  size_t GetSize() const { return mesh.size(); }

  value_type & GetNode(const node_id_t node_id) { return mesh[node_id]; }

  value_type & operator[](const node_id_t node_id) { return GetNode(node_id); }

  typename mesh_t<T, N>::iterator begin() { return std::begin(mesh); }

  typename mesh_t<T, N>::iterator end() { return std::end(mesh); }

  typename mesh_t<T, N>::const_iterator begin() const {
    return std::begin(mesh);
  }

  typename mesh_t<T, N>::const_iterator end() const {
    return std::end(mesh);
  }

};
