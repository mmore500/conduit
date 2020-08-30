#pragma once

#include "../../../third-party/Empirical/source/base/vector.h"

#include "../utility/print_utils.hpp"

#include "TopoNodeInput.hpp"
#include "TopoNodeOutput.hpp"

namespace uit {

class TopoNode {

  using inputs_t = emp::vector<uit::TopoNodeInput>;
  using outputs_t = emp::vector<uit::TopoNodeOutput>;

  inputs_t inputs;
  outputs_t outputs;

public:

  TopoNode(const inputs_t& inputs_, const outputs_t& outputs_)
  : inputs(inputs_), outputs(outputs_)
  { ; }

  const inputs_t& GetInputs() const { return inputs; }

  const outputs_t& GetOutputs() const { return outputs; }

  size_t GetNumInputs() const { return inputs.size(); }

  size_t GetNumOutputs() const { return outputs.size(); }

  bool HasInputs() const { return inputs.size(); }

  bool HasOutputs() const { return outputs.size(); }

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

};

} // namespace uit
