
#include <algorithm>
#include <numeric>
#include <type_traits>

namespace uitsl {

template<class InputIt>
constexpr void normalize(InputIt first, InputIt last) {

  using value_t = typename std::decay_t<decltype(*first)>;

  const value_t sum = std::accumulate( first, last, value_t{} );

  if (sum) std::transform(
    first, last, first, [sum](const value_t val){ return val/sum; }
  );

}

} // namespace uitsl
