#pragma once
#ifndef UITSL_MATH_STATS_UTILS_HPP_INCLUDE
#define UITSL_MATH_STATS_UTILS_HPP_INCLUDE

#include <cassert>
#include <stddef.h>
#include <vector>

#include "../../uit_emp/data/DataNode.hpp"
#include "../../uit_emp/math/Random.hpp"

namespace uitsl {

inline double sample_mean_with_replacement(
  uit_emp::Random & rand,
  const std::vector<double>& data
) {

  uit_emp::DataNode<double, uit_emp::data::Range> sampled;

  for (size_t sample = 0; sample < data.size(); ++sample) {
    sampled.Add(
      data[rand.GetUInt(data.size())]
    );
  }

  return sampled.GetMean();

}


inline std::tuple<double, double> bootstrap(
  uit_emp::Random & rand,
  const std::vector<double>& data,
  const double percentile=5.0,
  const size_t num_reps=10000
) {

  assert(percentile <= 50.0);

  uit_emp::DataNode<double, uit_emp::data::Log> sampled;

  for (size_t rep = 0; rep < num_reps; ++rep) {
    sampled.Add(
      sample_mean_with_replacement(rand, data)
    );
  }

  return {
    sampled.GetPercentile(percentile),
    sampled.GetPercentile(100.0 - percentile)
  };

}

} // namespace uitsl

#endif // #ifndef UITSL_MATH_STATS_UTILS_HPP_INCLUDE
