#pragma once

// TODO generalize

namespace uit {

template<
  template<typename, size_t> typename Input,
  size_t N
>
struct Curry {
  template<typename T>
  using curried = Input<T, N>;
};

}
