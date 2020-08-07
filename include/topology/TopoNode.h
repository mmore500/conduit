#pragma once

#include "TopoNodeInput.h"
#include "TopoNodeOutput.h"

#include "base/vector.h"

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

};

}
