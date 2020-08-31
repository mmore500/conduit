#pragma once

#include <iostream>
#include <algorithm>
#include <string_view>

#include "../../third-party/Empirical/source/base/vector.h"
#include "../../third-party/Empirical/source/polyfill/span.h"

#include "../utility/print_utils.hpp"
#include "../utility/stream_utils.hpp"

#include "TopoNodeInput.hpp"
#include "TopoNodeOutput.hpp"

namespace uit {

class TopoNode {

public:
  using input_t = uit::TopoNodeInput;
  using output_t = uit::TopoNodeOutput;

  using inputs_t = emp::vector<input_t>;
  using outputs_t = emp::vector<output_t>;

private:
  inputs_t inputs;
  outputs_t outputs;

public:

  TopoNode() = default;
  TopoNode(const inputs_t& inputs_, const outputs_t& outputs_)
  : inputs(inputs_), outputs(outputs_)
  { ; }

  const inputs_t& GetInputs() const noexcept { return inputs; }

  const outputs_t& GetOutputs() const noexcept { return outputs; }

  void AddInput(const uit::TopoNodeInput& input_) {
    inputs.push_back(input_);
  }

  void AddOutput(const uit::TopoNodeOutput& output_) {
    outputs.push_back(output_);
  }

  size_t GetNumInputs() const noexcept { return inputs.size(); }

  size_t GetNumOutputs() const noexcept { return outputs.size(); }

  bool HasInputs() const noexcept { return inputs.size(); }

  bool HasOutputs() const noexcept { return outputs.size(); }

  std::string ToString() const {
    std::stringstream ss;
    ss << format_member(
      "emp::vector<uit::TopoNodeInput> inputs",
      uit::to_string(inputs)
    ) << std::endl;
    ss << format_member(
      "emp::vector<uit::TopoNodeInput> inputs",
      uit::to_string(outputs)
    );
    return ss.str();
  }

  bool operator==(const TopoNode& other) const {
    return inputs == other.inputs && outputs == other.outputs;
  }

  friend std::ostream& operator<<(std::ostream&, const TopoNode&);
  friend std::istream& operator>>(std::istream&, TopoNode&);

};

std::ostream& operator<<(std::ostream& os, const TopoNode& node) {
  if (!node.HasOutputs()) return os;

  for (const auto& node : std::span<const TopoNode::output_t>(
    node.outputs.data(),
    node.outputs.size() - 1
  )) {
    os << node.GetEdgeID() << " ";
  }
  os << node.outputs.back().GetEdgeID();
  return os;
}

std::istream& operator>>(std::istream& is, TopoNode& node) {
  size_t input;
  while (is >> input) {
    node.AddInput(input);
  }
  return is;
}

};
