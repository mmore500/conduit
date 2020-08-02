#pragma once

#include "data/DataNode.h"
#include "base/vector.h"

namespace uit {

double sample_mean_with_replacement(
  emp::Random & rand,
  const emp::vector<double>& data
) {

  emp::DataNode<double, emp::data::Range> sampled;

  for (size_t sample = 0; sample < data.size(); ++sample) {
    sampled.Add(
      data[rand.GetUInt(data.size())]
    );
  }

  return sampled.GetMean();

}


std::tuple<double, double> bootstrap(
  emp::Random & rand,
  const emp::vector<double>& data,
  const double percentile=5.0,
  const size_t num_reps=10000
) {

  emp_assert(percentile <= 50.0);

  emp::DataNode<double, emp::data::Log> sampled;

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

}
