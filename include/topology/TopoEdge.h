#pragma once

#include "TopoNodeInput.h"
#include "TopoNodeOutput.h"

namespace uit {

class TopoEdge {

private:

  const size_t uid;

public:

  TopoEdge(const size_t uid_) : uid(uid_) { ; }

  TopoNodeOutput GetInlet() const { return TopoNodeOutput{uid}; }

  TopoNodeInput GetOutlet() const { return TopoNodeInput{uid}; }

};

}
