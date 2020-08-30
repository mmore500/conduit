#pragma once

namespace uit {

template<typename T>
class IdentityWrapper : public T {

  // inherit parent's constructors
  // adapted from https://stackoverflow.com/a/434784
  using T::T;

};

} // namespace uit
