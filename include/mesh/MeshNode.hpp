#pragma once

#include <stddef.h>
#include <optional>

#include "../../third-party/Empirical/source/base/vector.h"

#include "MeshNodeInput.hpp"
#include "MeshNodeOutput.hpp"

namespace uit {

template<typename ImplSpec>
class MeshNode {

public:
  using input_t = uit::MeshNodeInput<ImplSpec>;
  using output_t = uit::MeshNodeOutput<ImplSpec>;

private:
  using inputs_t = emp::vector<input_t>;
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

  size_t GetNodeID() const { return node_id; }

  inputs_t& GetInputs() { return inputs; }

  outputs_t& GetOutputs() { return outputs; }

  const inputs_t& GetInputs() const { return inputs; }

  const outputs_t& GetOutputs() const { return outputs; }

  input_t& GetInput(const size_t i) { return inputs[i]; }

  output_t& GetOutput(const size_t i) { return outputs[i]; }

  const input_t& GetInput(const size_t i) const { return inputs[i]; }

  const output_t& GetOutput(const size_t i) const { return outputs[i]; }

  size_t GetNumInputs() const { return inputs.size(); }

  size_t GetNumOutputs() const { return outputs.size(); }

  bool HasInputs() const { return GetNumInputs(); }

  bool HasOutputs() const { return GetNumOutputs(); }

  bool HasInput(const size_t i) const { return i < GetNumInputs(); }

  bool HasOutput(const size_t i) const { return i < GetNumOutputs(); }

  std::optional<input_t> GetInputOrNullopt(const size_t i) const {
    return HasInput(i) ? std::optional<input_t>{GetInput(i)} : std::nullopt;
  }

  std::optional<output_t> GetOutputOrNullopt(const size_t i) const {
    return HasOutput(i) ? std::optional<output_t>{GetOutput(i)} : std::nullopt;
  }

};

}
