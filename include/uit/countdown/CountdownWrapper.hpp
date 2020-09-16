#pragma once

#include <experimental/memory>

namespace uit {

template<typename CountdownType>
class CountdownWrapper {

  std::experimental::observer_ptr<CountdownType> impl;

public:

  CountdownWrapper() = default;

  CountdownWrapper(CountdownType& countdown) {
    impl = std::experimental::make_observer( &countdown );
  }

  bool operator!=(const CountdownWrapper other) {
    return ! impl->IsComplete();
  }

  CountdownWrapper& operator++() { ++*impl; return *this; }

  using elapsed_t = typename CountdownType::elapsed_t;
  elapsed_t operator*() const { return impl->GetElapsed(); }

};

} // namespace uit
