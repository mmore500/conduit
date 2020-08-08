#pragma once

#include <stddef.h>

#include "base/vector.h"

#include "MeshNodeInput.h"
#include "MeshNodeOutput.h"

namespace uit {

template<typename ImplSpec>
class MeshNode {


  using input_t = uit::MeshNodeInput<ImplSpec>;
  using inputs_t = emp::vector<input_t>;

  using output_t = uit::MeshNodeOutput<ImplSpec>;
  using outputs_t = emp::vector<output_t>;

  inputs_t inputs;
  outputs_t outputs;

  const size_t node_id;

public:

  MeshNode(const size_t node_id_) : node_id(node_id_) { ; }

  void AddInput(const input_t &input) {
    inputs.push_back(input);
  }

  void AddOutput(const output_t &output) {
    outputs.push_back(output);
  }

  inputs_t& GetInputs() { return inputs; }

  outputs_t& GetOutputs() { return outputs; }

  const inputs_t& GetInputs() const { return inputs; }

  const outputs_t& GetOutputs() const { return outputs; }

  size_t GetNodeID() const { return node_id; }

};

}
