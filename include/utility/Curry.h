#pragma once

#include <stddef.h>
#include <type_traits>
// TODO generalize

namespace uit {

#include <assert.h>
#include <vector>

template<
  template<typename...> typename Input,
  typename Specified
>
struct CurryTypes {

  template<typename... Rest>
  using curried = Input<Rest..., Specified>;

};

template<
  template<auto...> typename Input,
  auto Specified
>
struct CurryValues {

  template<auto... Rest>
  using curried = Input<Rest..., Specified>;

};

template<
  template<typename...> typename Input,
  typename Specified
>
using Curry = CurryTypes<Input, Specified>;

template<auto x>
using ValType=std::integral_constant<decltype(x), x>;

}
