#pragma once

// TODO generalize

template<
  template<typename, size_t> typename Input,
  size_t N
>
struct Curry {
  template<typename T>
  using curried = Input<T, N>;
};
