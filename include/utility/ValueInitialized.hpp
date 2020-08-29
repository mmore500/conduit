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

};

} // namespace uit
