#pragma once

#include <algorithm>
#include <memory>

#include "../../../third-party/Empirical/include/emp/math/Random.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

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
  const size_t num_nodes;

  size_t state{};

  const double b;

  const size_t c; // number of available channels
  emp::vector<double> p; // state vector


  emp::Random rand;

  void PullInputs() {
    std::vector<size_t> values;
    values.reserve( inputs.size() );
    std::transform(
      std::begin(inputs),
      std::end(inputs),
      std::back_inserter(values),
      [this](auto& input){
        received_message_counter += input.Jump();
        return input.Get();
      }
    );

    const size_t plurality = uitsl::get_plurality(
      std::begin(values),
      std::end(values)
    );

    // ignore value-initialized initial Gets
    if (plurality) state = plurality;

    auto channel_picker = [&]() -> size_t {
      const double d = rand.GetDouble();

      double sum = p[0];

      // turn to index map
      for (size_t i = 0; i < p.size(); ++i) {
        if (d < sum) return i;
        sum += p[i + 1];
      }
    };

    // choose channel with probability p
    const size_t channel = channel_picker();

    if(!HasInterference(channel, values)) {
      for (size_t i = 0; i < p.size(); ++i) {
        if (i == channel) p[i] = 1;
        else p[i] = 0;
      }
      // choose channel
      state = channel;
    } else {
      for (size_t i = 0; i < p.size(); ++i) {
        if (i == channel) p[i] *= (1 - b);
        else p[i] = (1 - b) * p[i] + b / (c - 1);
      }
    }
  }

  void PushOutputs() {
    for (auto& out : outputs) {
      out.Put( state );
      out.TryFlush();
      ++sent_message_counter;
    }
  }

public:

  Cell(const node_t& node, const size_t num_nodes_)
  : inputs( node.GetInputs() )
  , outputs( node.GetOutputs() )
  , node_id( node.GetNodeID() )
  , num_nodes( num_nodes_ )
  , state( node_id + 1 )
  , b(0.1)
  , c(2)
  , p(c, 1.0/c)
  { }

  void Update() {
    PullInputs();
    PushOutputs();
  }

  size_t GetNumMessagesSent() const { return sent_message_counter; }

  size_t GetNumMessagesReceived() const { return received_message_counter; }

  const size_t GetNodeID() const { return node_id; }

  double GetState() const { return state; }

  bool HasInterference(const size_t channel, const std::vector<size_t>& values) const {
    return std::find(
      values.begin(),
      values.end(),
      channel
    ) != values.end();
  }

};
