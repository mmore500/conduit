#pragma once

#include "../third-party/Empirical/source/tools/string_utils.h"

class State {

  char state;

public:

  State() : state('_') { ; }

  State(const char & x) : state(x) { ; }

  operator char() const { return state; }

  std::string ToString() const { return emp::to_string(state); }

};
