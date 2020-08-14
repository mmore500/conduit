#pragma once

#include <memory>
#include <stddef.h>
#include <utility>

#include "Duct.hpp"
#include "Inlet.hpp"

namespace uit {

template<typename ImplSpec>
class Sink {

  uit::Inlet<ImplSpec> inlet{
    std::make_shared<uit::Duct<ImplSpec>>()
  };

public:

  // for structured bindings
  template <size_t N>
  decltype(auto) get() const {
      // parens needed to get reference?
      if constexpr (N == 0) return (inlet);
  }

  uit::Inlet<ImplSpec>& GetInlet() {
    return inlet;
  }

  // TODO implicit conversion operator?

};

}

// for structured bindings
namespace std {

  template<typename ImplSpec>
  struct tuple_size<uit::Sink<ImplSpec>>
    : std::integral_constant<size_t, 1>{};

  template<typename ImplSpec, size_t N>
  struct tuple_element<N, uit::Sink<ImplSpec>> {

    using type = decltype(
      std::declval<uit::Sink<ImplSpec>>().template get<N>()
    );
  };

}
