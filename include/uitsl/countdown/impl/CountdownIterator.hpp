#pragma once

#include <experimental/memory>

namespace uitsl {

template<typename CountdownType>
class CountdownIterator {

  std::experimental::observer_ptr<CountdownType> impl;

public:

  CountdownIterator() = default;

  CountdownIterator(CountdownType& countdown) {
    impl = std::experimental::make_observer( &countdown );
  }

  bool operator!=(const CountdownIterator other) {
    return ! impl->IsComplete();
  }

  CountdownIterator& operator++() { ++*impl; return *this; }

  using elapsed_t = typename CountdownType::elapsed_t;
  elapsed_t operator*() const { return impl->GetElapsed(); }

};

} // namespace uitsl
