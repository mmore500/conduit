#pragma once

namespace uit {

template<typename T>
class ValueInitialized {

  T val{};

public:

  ValueInitialized() { ; }
  ValueInitialized(const T& val_) : val(val_) { ; }

  operator const T&() const { return val; }
  operator T&() { return val; }

  T* operator&() { return &val; }

  // TODO cpp20 spaceship operator
  // https://devblogs.microsoft.com/cppblog/simplify-your-code-with-rocket-science-c20s-spaceship-operator/
  bool operator==(const T& rhs) const { return val == rhs; }

  bool operator!=(const T& rhs) const { return val != rhs; }

};

} // namespace uit
