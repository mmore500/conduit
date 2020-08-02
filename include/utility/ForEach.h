#pragma once

#include <functional>
// plus
// minus
// multiplies
// divides
// modulus
// negate


template <
  template<size_t> typename Payload,
  size_t N,
  size_t Operand=1,
  typename Afterthought=std::plus<size_t>,
  typename Comparison=std::less<size_t>
  >
struct ForEach {

  template <size_t I>
  void item() {

    if constexpr ( !Comparison{}(I, N) ) Payload<I>{};

    constexpr size_t next = Afterthought{}(
      I,
      Operand
    );

    // recurse upwards
    if constexpr ( !Comparison{}(I, N) ) ForEach<
      Payload,
      N,
      Operand,
      Afterthought,
      Comparison
    >{}.template item<next>();

  }

};
