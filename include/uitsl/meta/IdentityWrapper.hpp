#pragma once

namespace uitsl {

template<typename T>
class IdentityWrapper : public T {

  // inherit parent's constructors
  // adapted from https://stackoverflow.com/a/434784
  using T::T;

};

} // namespace uitsl
