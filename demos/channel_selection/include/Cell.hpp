#pragma once

#include <algorithm>
#include <memory>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/Random.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"

#include "uitsl/algorithm/get_plurality.hpp"

#include "typedef.hpp"

class Cell {

  size_t received_message_counter{};
  size_t sent_message_counter{};

  using inputs_t = node_t::inputs_t;
  inputs_t inputs;

  using outputs_t = node_t::outputs_t;
  outputs_t outputs;

  size_t node_id;

  size_t set_channel{};

  emp::vector<double> p; // state vector

  size_t ProposeNextChannel() {

    thread_local emp::Random rand;

    const double d = rand.GetDouble();

    double sum = p[0];

    // turn to index map
    for (size_t i{}; i < p.size(); ++i) {
      if (d < sum) return i;
      sum += p[i + 1];
    }

    emp_always_assert(false);
    __builtin_unreachable();

  }

  bool DetectInterference(const size_t proposed_channel) const {
    return std::any_of(
      std::begin( inputs ), std::end( inputs ),
      [proposed_channel]( const auto& input ){
        return input.Get() == proposed_channel;
      }
    );
  }

  void PullInputs() {
    for (auto& in : inputs) received_message_counter += in.Jump();
  }

  void PushOutputs() {
    for (auto& out : outputs) {
      sent_message_counter += out.TryPut( set_channel );
      out.TryFlush();
    }
  }

  void UpdateSetChannel() {

    const double b = cfg.B();
    const size_t c = inputs.size();

    thread_local std::vector<size_t> neighbor_channels;
    neighbor_channels.resize( inputs.size() );
    std::transform(
      std::begin(inputs),
      std::end(inputs),
      std::begin(neighbor_channels),
      [](const auto& input){ return input.Get(); }
    );

    // choose channel with probability p
    const size_t proposed_channel = ProposeNextChannel();

    if( !DetectInterference(proposed_channel) ) {
      for (size_t i = 0; i < p.size(); ++i) p[i] = (i == proposed_channel);

      // choose proposed_channel
      set_channel = proposed_channel;
    } else {
      for (size_t i = 0; i < p.size(); ++i) {
        if (i == proposed_channel) p[i] *= (1 - b);
        else p[i] = (1 - b) * p[i] + b / (c - 1);
      }
    }
  }

public:

  Cell(const node_t& node)
  : inputs( node.GetInputs() )
  , outputs( node.GetOutputs() )
  , node_id( node.GetNodeID() )
  , p( inputs.size(), 1.0 / inputs.size() )
  { }

  void Update() {
    PullInputs();
    UpdateSetChannel();
    PushOutputs();
  }

  size_t GetNumMessagesSent() const { return sent_message_counter; }

  size_t GetNumMessagesReceived() const { return received_message_counter; }

  const size_t GetNodeID() const { return node_id; }

  double GetState() const { return set_channel; }

};
