#pragma once

namespace uitsl {

template<typename Ratio>
constexpr double ratio_to_double() {
  return Ratio::num / static_cast<double>( Ratio::den );
}

template<typename Ratio>
constexpr double as_double() { return ratio_to_double<Ratio>(); }

}
